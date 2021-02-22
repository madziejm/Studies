// Lista 10 -- zadanie 3 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define DRIVER_DDR  DDRD
#define DRIVER_PORT PORTD
#define DRIVER_EN_DDR DDRB
#define DRIVER_EN   PB1 // OC1A
#define DRIVER_1A   PD2
#define DRIVER_2A   PD3

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


void timer_init()
{
  TCCR1A = _BV(COM1A1) | _BV(WGM11); // Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
  TCCR1B = _BV(WGM13) | _BV(WGM12); // Fast PWM Mode 14
  // Prescaler 8
  TCCR1B |= _BV(CS11);
  ICR1 = 2 * 1023; // top
  // 1000 Hz ~=(16e6/(8 * (1 + 2046)))
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
  DRIVER_EN_DDR |= _BV(DRIVER_EN);
  DRIVER_DDR |= _BV(DRIVER_1A);
  DRIVER_DDR |= _BV(DRIVER_2A);
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  adc_init();

  // DRIVER_PORT |= _BV(DRIVER_EN);

  sei();

  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersj<C4><99>
    // OCR1A = adc;
    int16_t a;
    if(adc < 512)
    {
      a = (511 - (int16_t)adc) * 4;
      OCR1A = a;
      DRIVER_PORT &= ~_BV(DRIVER_1A);
      DRIVER_PORT |= _BV(DRIVER_2A);
    }
    else
    {
      a = (adc - 512) * 4;
      OCR1A = a;
      DRIVER_PORT &= ~_BV(DRIVER_2A);
      DRIVER_PORT |= _BV(DRIVER_1A);
    }
    float VCC = 5.0 / 1023.0 * (float)adc;
    printf("potentiometer: %04"PRIu16"/1023;  %.5f V OCR1A %"PRId16"\r\n", adc, VCC, a);
  }
}

