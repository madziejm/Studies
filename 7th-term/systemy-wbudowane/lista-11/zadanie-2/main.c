#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "pid.h"

#define K_P     0.5 // 0.60
#define K_I     0.3 // 0.23
#define K_D     0.2

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
  ADMUX   = _BV(REFS0); // nap. referencyjne 5 V, wejście analogowe ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  DIDR0  |= _BV(ADC1D); // wyłącz wejście cyfrowe na ADC1
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // enable ADC
  ADCSRA |= _BV(ADIE);
}


void timer_init()
{
  TCCR1A = _BV(COM1A1); // Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
  TCCR1B = _BV(WGM13); // PWM, Phase and Frequency Correct (Mode 8)
  // Prescaler 8
  TCCR1B |= _BV(CS11);
  ICR1 = 1023; // top
  // 1000 Hz ~=(16e6/(2 * 8 * (1 + 1023)))
  TIMSK1 =  _BV(ICIE1);
}


FILE uart_file;

ISR(TIMER1_CAPT_vect) // 
{
  ADMUX |= _BV(MUX0); // wejście analogowe ADC1 -- silnik
  if(! (ADCSRA & _BV(ADSC)))
    ADCSRA |= _BV(ADSC); // start conversion
}

uint16_t adc0 = 0;
uint16_t adc1 = 0;

/*! \brief Flags for status information
 */
struct GLOBAL_FLAGS {
  //! True when PID control loop should run one time
  uint8_t pidTimer:1;
  uint8_t dummy:7;
} gFlags = {0, 0};

ISR(ADC_vect)
{
  if(ADMUX & _BV(MUX0)) // ADC1 analog input -- motor
  {
    adc1  = ADC;
    ADMUX &= ~_BV(MUX0); // ADC0 analog input -- potentiometer
    ADCSRA |= _BV(ADSC); // start new potentiometer conversion // comment me?
  }
  else // ADC0 analog input -- potentiometer
  {
    adc0  = ADC;
    ADMUX |= _BV(MUX0); // ADC1 analog input -- motor
    gFlags.pidTimer = TRUE;
  }
}

//! Parameters for regulator
struct PID_DATA pidData;

int main(void)
{
  uart_init();
  timer_init();

  MOTOR_DDR |= _BV(MOTOR);
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  adc_init();

  int16_t referenceValue, measurementValue, inputValue = 64;
  pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);

  sei();

  while(1) {
    float potentiometer_VCC = 5.0 / 1023.0 * (float)adc0;
    printf("potentiometer: %04"PRIu16"/1023;  %.5f V ", adc0, potentiometer_VCC);
    float motor_VCC = 5.0 / 1023.0 * (float)adc1;
    printf("motor: %04"PRIu16"/1023;  %.5f V\r\n", adc1, motor_VCC);
    _delay_ms(100);

    if(gFlags.pidTimer)
    {
      referenceValue = adc0; 
      measurementValue = 1203 - adc1;
      int16_t pid = pid_Controller(referenceValue, measurementValue, &pidData);
      inputValue += pid;
      inputValue = inputValue < 0? 0 : inputValue;
      printf("reference:  %"PRId16", measurement:  %"PRId16", pid: %"PRId16", input:  %"PRId16"\r\n", referenceValue, measurementValue, pid, inputValue);

      OCR1A = inputValue;

      gFlags.pidTimer = FALSE;
    }
  }
  return 0;
}

