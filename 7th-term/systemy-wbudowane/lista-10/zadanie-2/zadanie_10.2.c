// Lista 10 -- zadanie 2
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

// #define __DELAY_BACKWARD_COMPATIBLE__
// #include <util/delay.h>

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
  ADMUX  = _BV(REFS0) | _BV(MUX0); // nap. referencyjne 5 V, wejście analogowe ADC1
  DIDR0  = _BV(ADC0D) | _BV(ADC1D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // | _BV(ADATE); // włącz ADC oraz auto trigger enable
  ADCSRA |= _BV(ADIE);
  // ADCSRB  = _BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0); // Trigger Source: Timer/Counter1 Capture Event // włączyć to
}


void timer_init()
{
  TCCR1A = _BV(COM1A1); // Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
  TCCR1B = _BV(WGM13); // PWM, Phase and Frequency Correct (Mode 8)
  // Prescaler 8
  TCCR1B |= _BV(CS11);
  ICR1 = 1023; // top
  // 1000 Hz ~=(16e6/(2 * 8 * (1 + 1023)))s
  TIMSK1 =  _BV(ICIE1); // interrupt when counter matches OCR1A
  TIMSK1 |= _BV(TOIE1);  // timer overflow interrupt enable
}


FILE uart_file;

uint16_t adc0 = 0;
uint16_t adc1o = 0;
uint16_t adc1c = 0;

enum measurement {TRANSISTOR_CLOSED, TRANSISTOR_OPENED, POTENTIOMETER};

enum measurement m = {TRANSISTOR_CLOSED};

ISR(TIMER1_CAPT_vect)
{
  m = TRANSISTOR_CLOSED;
  ADMUX |= _BV(MUX0);
  ADCSRA |= _BV(ADSC);
}

ISR(TIMER1_OVF_vect)
{
  m = TRANSISTOR_OPENED;
  ADMUX |= _BV(MUX0);
  ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect)
{
  if(ADMUX & _BV(MUX0))
  {
    if(TRANSISTOR_CLOSED == m)
    {
      adc1c = ADC;
    }
    else if(TRANSISTOR_OPENED == m)
    {
      adc1o = ADC;
    }
    ADMUX &= ~_BV(MUX0);
    m = POTENTIOMETER;
    ADCSRA |= _BV(ADSC); 
  }
  else if (POTENTIOMETER == m)
  {
    OCR1A = adc0 = ADC;
    ADMUX |= _BV(MUX0);
  }
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
    float VCC_p = 5.0 / 1023.0 * (float)adc0;
    float VCC_o = 5.0 / 1023.0 * (float)adc1o;
    float VCC_c = 5.0 / 1023.0 * (float)adc1c;
    printf("potentiometer: %04"PRIu16"/1023;  %.5f V\r\n", adc0, VCC_p);
    printf("motor: %04"PRIu16"/1023;  %.5f V\r\n", adc1o, VCC_o);
    printf("motor: %04"PRIu16"/1023;  %.5f V\r\n", adc1c, VCC_c);
  }
}

