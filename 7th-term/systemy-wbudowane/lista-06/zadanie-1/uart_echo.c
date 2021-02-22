#include <avr/io.h>
// #include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

char byte;

ISR(USART_RX_vect) // USART Receive Complete
{
  byte = UDR0;
  UCSR0B |= _BV(UDRIE0); // ready to receive interrupt enable
}

ISR(USART_UDRE_vect) // USART Data Register Empty (the transmit buffer (UDRn) is ready to receive new data)
{
  UCSR0B &= ~_BV(UDRIE0); // ready to receive interrupt disable
  UDR0 = byte;
  if('\r' == byte)
  {
	  while(!(UCSR0A & _BV(UDRE0)));
	  UDR0 = '\n';
  }
}

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0B |= _BV(RXCIE0); // receive interrupt enable
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int main()
{
  uart_init();
  sei();
  set_sleep_mode(SLEEP_MODE_IDLE);
  while(1) {
    sleep_mode();
  }
}

