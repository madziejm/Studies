// Lista 10 -- zadanie 1 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define MOTOR      PB1
#define MOTOR_DDR  DDRB
#define MOTOR_PORT PORTB

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

uint16_t adc = 0;

ISR(ADC_vect)
{
  adc = ADC;
  OCR1A = adc * 2;
}

int main()
{
  uart_init();
  timer_init();
  MOTOR_DDR |= _BV(MOTOR);
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  adc_init();

  MOTOR_PORT |= _BV(MOTOR);

  sei();

  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersj<C4><99>
    float VCC = 5.0 / 1023.0 * (float)adc;
    printf("potentiometer: %04"PRIu16"/1023;  %.5f V\r\n", adc, VCC);
  }
}

