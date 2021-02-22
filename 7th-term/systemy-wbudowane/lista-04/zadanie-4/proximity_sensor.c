#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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
  return UDR0;
}

FILE uart_file;

void timer1_init()
{
  // 37914.69194312796 Hz (16000000/((421+1)*1)), ale robimy toggle
  OCR1A = 210;
  TCCR1A = _BV(COM1A0); // toggle OC1A
  TCCR1B = _BV(WGM12) | _BV(CS10); // CTC, no prescalling
}

#define LED_PORT      PORTB
#define LED_DDR       DDRB
#define LED           PB5

#define IR_PORT       PORTB
#define IR_DDR        DDRB
#define IR            PB1

#define DETECTOR_PIN  PINB
#define DETECTOR_PORT PORTB
#define DETECTOR_DDR  DDRB
#define DETECTOR      PB0

int main()
{
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  LED_DDR |= _BV(LED);
  DETECTOR_DDR  &= ~_BV(DETECTOR); // wejście, nie trzeba
  DETECTOR_PORT |= _BV(DETECTOR); // pull up
  timer1_init();
  while(1)
  {
    IR_DDR |= _BV(IR);
    _delay_us(600);
    if(DETECTOR_PORT & _BV(DETECTOR_PIN))
      LED_PORT |= _BV(LED);
    else
      LED_PORT &= ~_BV(LED);
    printf("%"PRId8"\r\n", DETECTOR_PIN & _BV(DETECTOR));
    IR_DDR &= ~_BV(IR);
    _delay_us(100);
  }
}

