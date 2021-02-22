#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "i2c.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
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

const uint8_t eeprom_addr = 0b11010000;

#define WRITE 0
#define READ 1

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  i2cInit();
  #define i2cCheck(code, msg) \
    if ((TWSR & 0xf8) != (code)) { \
      printf(msg " failed, status: %.2x\r\n", TWSR & 0xf8); \
      i2cReset(); \
      continue; \
    }

  while(1)
  {
    char command_buf[20];

    scanf("%s ", command_buf);
    printf("Received command %s\r\n", command_buf);

    if(0 == strcmp(command_buf, "date"))
    {
      uint8_t month_day, month, year;
      while(1)
      {
	i2cStart();
	i2cCheck(0x08, "I2C start in read");
	i2cSend(eeprom_addr);
	i2cCheck(0x18, "I2C EEPROM write request")
	i2cSend(0x04);
	i2cCheck(0x28, "I2C EEPROM set address")
	i2cStart();
	i2cCheck(0x10, "I2C second start")
	i2cSend(eeprom_addr | READ);
	month_day = i2cReadAck();
	i2cCheck(0x50, "I2C EEPROM read request")
	month = i2cReadAck();
	i2cCheck(0x50, "I2C EEPROM read request")
	year = i2cReadNoAck();
	i2cCheck(0x58, "I2C EEPROM read request")
	i2cStop();
	i2cCheck(0xf8, "I2C stop")
	// printf("Read data: addr[0x%.3x] = 0x%x\r\n", command_address, data);
	break;
      }
      printf("%.2"PRIx8"-%.2"PRIx8"-%.2"PRIx8"%.2"PRIx8"\r\n", month_day, month & 0x1f, month & 0x80? 0x21: 0x20, year);
    }

    if(0 == strcmp(command_buf, "time"))
    {
      uint8_t seconds, minutes, hours;
      while(1)
      {
	i2cStart();
	i2cCheck(0x08, "I2C start in read");
	i2cSend(eeprom_addr);
	i2cCheck(0x18, "I2C EEPROM write request")
	i2cSend(0x0);
	i2cCheck(0x28, "I2C EEPROM set address")
	i2cStart();
	i2cCheck(0x10, "I2C second start")
	i2cSend(eeprom_addr | READ);
	seconds = i2cReadAck();
	i2cCheck(0x50, "I2C EEPROM read request")
	minutes = i2cReadAck();
	i2cCheck(0x50, "I2C EEPROM read request")
	hours = i2cReadNoAck();
	i2cCheck(0x58, "I2C EEPROM read request")
	i2cStop();
	i2cCheck(0xf8, "I2C stop")
	break;
      }
      printf("%.2"PRIx8":%.2"PRIx8":%.2"PRIx8"\r\n", hours, minutes, seconds);
    }

    if(0 == strcmp(command_buf, "set"))
    {
      scanf("%s ", command_buf);
      printf("Received command %s\r\n", command_buf);
      if(0 == strcmp(command_buf, "date"))
      {
	uint8_t month_day, month, century, year;
	scanf("%"SCNx8"-%"SCNx8"-%2"SCNx8"%2"SCNx8" ", &month_day, &month, &century, &year);
	while(1)
	{
	  i2cStart();
	  i2cCheck(0x08, "I2C start in write")
	  i2cSend(eeprom_addr | WRITE);
	  i2cCheck(0x18, "I2C EEPROM write request in write")
	  i2cSend(0x04);
	  i2cCheck(0x28, "I2C EEPROM set address in write")
	  i2cSend(month_day);
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cSend(month | (century == 0x21? 0x80 : 0));
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cSend(year);
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cStop();
	  i2cCheck(0xf8, "I2C stop")
	  break;
	}
	printf("date set\r\n");
       }

      if(0 == strcmp(command_buf, "time"))
      {
	uint8_t seconds, minutes, hours;
	scanf("%"SCNx8":%"SCNx8":%"SCNx8" ", &hours, &minutes, &seconds);
	while(1)
	{
	  i2cStart();
	  i2cCheck(0x08, "I2C start in write")
	  i2cSend(eeprom_addr | WRITE);
	  i2cCheck(0x18, "I2C EEPROM write request in write")
	  i2cSend(0);
	  i2cCheck(0x28, "I2C EEPROM set address in write")
	  i2cSend(seconds);
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cSend(minutes);
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cSend(hours);
	  i2cCheck(0x28, "I2C EEPROM write")
	  i2cStop();
	  i2cCheck(0xf8, "I2C stop")
	  break;
	}
	printf("time set\r\n");
      }
    }
    printf("\r\n");
  }
}

