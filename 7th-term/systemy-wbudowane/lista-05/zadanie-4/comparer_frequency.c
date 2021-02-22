#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

FILE uart_file;

volatile uint16_t last_capture;
volatile uint16_t capture;
volatile uint16_t period;
volatile uint16_t frequency;

// timer 1 capture event isr
ISR(TIMER1_CAPT_vect) 
{
  // if(TCCR1B | _BV(ICES1))  // rising edge triggered the capture
  {
    last_capture = capture;
    capture = ICR1;
    // if(last_capture < capture) // prevent overflow
    {
      period = capture - last_capture;
      if(period) // no zero division
	frequency = 16e6 / 256 / period;
    }
  }
  // TCCR1B ^= _BV(ICES1); // interrupt on rising/falling edge
}

// timer 1 overflow isr
ISR(TIMER1_OVF_vect) 
{
  last_capture = capture = 0;
}

void timer_1_init()
{
  // normal mode
  // input capture
  // TCCR1A = ; // not needed
  TCCR1B = _BV(ICES1) | _BV(CS12); // prescaler 256 (62,5 KHz)
  // ICES1: Input Capture Edge Select (1 -> rising edge)
  TCCR1B |= _BV(ICNC1); // ICNC1: Input Capture Noise Canceler
  TIMSK1 = _BV(ICIE1) | _BV(TOIE1); 
  // ICIE1: Timer/Counter1, Input Capture Interrupt Enable
  // TOIE1: Timer/Counter1, Overflow Interrupt Enable
  sei();
}

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  timer_1_init();
  // set_sleep_mode(SLEEP_MODE_IDLE); // not needed - idle sleep
  while(1) {
    sleep_cpu();
    printf("frequency=%"PRIu16 "  \r\n", frequency);
    sleep_mode();
    _delay_ms(100);
  }
}

