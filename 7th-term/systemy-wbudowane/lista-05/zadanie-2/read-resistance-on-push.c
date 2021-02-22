#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define BTN PORTD2 // INT0
#define BTN_PORT PORTD

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128, interrupt on end
  ADCSRA |= _BV(ADEN); // włącz ADC
}

float r_photoresistor;

FILE uart_file;

void set_button_on_push_interrupt()
{
  EICRA |= _BV(ISC00) | _BV(ISC01); // przerwanie: zbocze narastające na INT0
  EIMSK |= _BV(INT0);
}

ISR(INT0_vect)
{
  // mierz napięcie
  ADCSRA |= _BV(ADSC) | _BV(ADATE) | _BV(ADIE); // wykonaj konwersję uruchamiając przerwania
  // while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
}

ISR(ADC_vect)
{
  ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
  uint16_t adc = ADC;
  float k = adc / 1023.0;
  const float r_resistor = 10e3; // resistor 10 kOhm
  r_photoresistor = r_resistor * (1.0 - k) / k;
  ADCSRA &= ~(_BV(ADATE) | _BV(ADIE)); // wyłącz przerwania
}

int main()
{
  BTN_PORT |= _BV(BTN); // pull up
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  set_button_on_push_interrupt();
  sei();
  while(1) {
    printf("opór fotorezystora: %f\n\r", r_photoresistor);
    // printf("adc: %d, opór fotorezystora: %f\n\r", adc, r_photoresistor);
    _delay_ms(1000);
  }
}

