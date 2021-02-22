#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem


FILE uart_file;

char    in_buffer[256];
uint8_t in_start = 0;  // first element
uint8_t in_end = 0;    // last element
char    out_buffer[256];
uint8_t out_start = 0; // first element
uint8_t out_end = 0;   // past last element

ISR(USART_RX_vect) // USART Receive Complete
{
  char byte = UDR0;
  if((uint8_t)(in_end + 1) == in_start) // in buffer full
    in_start = (in_start + 1); // drop one element
  in_buffer[in_end++] = byte;
}

ISR(USART_TX_vect) // USART Transmit Complete
{
}

ISR(USART_UDRE_vect) // USART Data Register Empty (the transmit buffer (UDR) is ready to receive new data)
{
  if(out_start == out_end) // out buffer empty
  {
    UCSR0B &= ~_BV(UDRIE0);
  }
  else // out buffer nonempty
  {
    UDR0 = out_buffer[out_start];
    out_start = (out_start + 1);
  }
}

// transmisja jednego znaku
int uart_transmit(char byte, FILE *stream)
{
  // do{
  //   cli();
  //   if((uint8_1)(out_end + 1) == out_start) // buffer full
  //   {
  //     UCSR0B |= _BV(UDRIE0); // ready to receive data interrupt enable
  //     sei();
  //     sei();
  //     continue;
  //   }
  // }while(0);
  cli();
  while((uint8_t)(out_end + 1) == out_start)
  {
    UCSR0B |= _BV(UDRIE0); // ready to receive data interrupt enable
    sei();
    sei();
    cli();
  }
  UCSR0B |= _BV(UDRIE0); // ready to receive data interrupt enable
  out_buffer[out_end] = byte;
  out_end = (out_end + 1);
  sei();
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  cli();
  while(in_end == in_start) // buffer empty
  {
    sei();
    sei();
    cli();
  }
  char byte = in_buffer[in_start];
  in_start = (in_start + 1);
  sei();
  return byte;
}

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0B |= _BV(RXCIE0); // _BV(UDRIE0); // _BV(TXCIE0);// receive/transmit complete, UARTdata register empty interrupt enable
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
  sei();
}

int main()
{
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  char buf[250];
  printf("Jak się nazywasz? ");
  scanf("%s", buf);
  while(1) {
    printf("Cześć, %s\r\n", buf);
  }
}

