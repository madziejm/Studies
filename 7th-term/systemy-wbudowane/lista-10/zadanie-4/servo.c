// Lista 10 -- zadanie 4
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define SERVO_DDR DDRB
#define SERVO   PB1 // OC1A

#define POTENTIOMETER

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  char received = UDR0;
  uart_transmit(received, NULL);
  return received;
}

void adc_init()
{
  ADMUX  = _BV(REFS0); // nap. referencyjne 5 V, wejście analogowe ADC0
  DIDR0  = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // ADC on
  ADCSRA |= _BV(ADIE); // ADC interrupt
}

#define LEFT     32 //  37 // 37,46 / 1249 * (1000 / 50 ms) = 0,59 ms
#define NEUTRAL  87 //  87 // (..) 1,4 ms
#define RIGHT   137 // 137 // (..) 2,2

void timer_init()
{
  TCCR1A = _BV(COM1A1) | _BV(WGM11); // Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
  TCCR1B = _BV(WGM13) | _BV(WGM12); // Fast PWM Mode 14
  // Prescaler 12
  TCCR1B |= _BV(CS12);
  ICR1 = 1249; // top
  OCR1A = NEUTRAL;
  // 50 Hz ~=(16e6/(256 * (1 + 1249)))
}

FILE uart_file;

int16_t adc = 0;

ISR(ADC_vect)
{
  adc = ADC;
}

int main()
{
  uart_init();
  timer_init();

  SERVO_DDR |= _BV(SERVO);
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  adc_init();

  sei();

  int16_t step = 1;

  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersj<C4><99>
    float VCC = 5.0 / 1023.0 * (float)adc;
#ifdef POTENTIOMETER
    OCR1A = LEFT + adc * (1.0 / 1023.0) * (RIGHT - LEFT);
#else
    if(RIGHT < OCR1A + step || OCR1A + step < LEFT)
      step = -step;
    OCR1A += step;
#endif
    printf("potentiometer: %04"PRIu16"/1023;  %.5f V OCR1A: %"PRIu16  "\r\n", adc, VCC, OCR1A);
    // _delay_ms(5);
  }
}

