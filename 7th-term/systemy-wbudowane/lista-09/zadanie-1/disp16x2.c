// Lista 9 - zadanie 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define CURSOR_CHAR_CODE 0xff
#define CURSOR_STRING "\xff"
#define CURSOR_CHAR '\xff'

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczy<C5>?<C4><87> rejestr UCSR0A
  UCSR0A = 0;
  // w<C5><82><C4><85>cz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void uart_transmit(char data)
{
  // czekaj a<C5><BC> transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
}

int uart_receive(FILE *stream)
{
  // czekaj a<C5><BC> znak dost<C4><99>pny
  while (!(UCSR0A & _BV(RXC0)));
  char received = UDR0;
  uart_transmit(received); // echo
  return received;
}

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_uart_file;

#define LINE_LENGTH 16
#define INITIALIZE_LINE(buffer) do{ for(int i = 0; i < LINE_LENGTH; i++) buffer[i] = ' '; buffer[LINE_LENGTH] = '\0'; } while(0)
#define PRINT_ACTIVE_LINE(buffer, cursor_position) do{ for(int i = 0; i < cursor_position; i++) putc(buffer[i], &hd44780_uart_file);  putc(CURSOR_CHAR, &hd44780_uart_file); for(int i = cursor_position + 1; i < LINE_LENGTH; i++) putc(' ', &hd44780_uart_file); } while(0)
#define PRINT_LINE(buffer) do{\
  for(int i = 0; i < LINE_LENGTH; i++)\
  if(buffer[i])\
    putc(buffer[i], &hd44780_uart_file);\
  else\
    while(i++ < LINE_LENGTH)\
      putc(' ', &hd44780_uart_file);\
} while(0)


int main()
{
  LCD_Initialize();
  LCD_Clear();
  uart_init();
  fdev_setup_stream(&hd44780_uart_file, hd44780_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &hd44780_uart_file;
  char lines[2][LINE_LENGTH + 1];
  INITIALIZE_LINE(lines[0]);
  INITIALIZE_LINE(lines[1]);
  int active_line = 0;
  int cursor_position = 0;
  while(1) 
  {
    char* current_line = lines[active_line];
    char* last_line    = lines[active_line ^ 1];

    LCD_GoTo(0, 0);
    PRINT_LINE(last_line);
    LCD_GoTo(0, 1);
    PRINT_ACTIVE_LINE(current_line, cursor_position);

    char read_char = getchar();

    current_line[cursor_position++] = read_char == '\r'? '\0' : read_char;

    if(16 <= cursor_position || read_char == '\r')
    {
      cursor_position = 0;
      active_line ^= 1;
    }
  }
}

