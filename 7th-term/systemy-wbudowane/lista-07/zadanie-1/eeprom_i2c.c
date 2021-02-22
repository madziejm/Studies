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

const uint8_t eeprom_addr = 0xa0; // 0b1010...

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
    uint16_t command_address;
    uint8_t data;

    scanf("%s %"SCNx16, command_buf, &command_address);
    printf("Received command %s; command_address=0x%"PRIx16"\r\n", command_buf, command_address);
    
    if(0 == strcmp(command_buf, "read"))
    {
      while(1)
      {
	i2cStart();
	i2cCheck(0x08, "I2C start in read");
	// 4K 24C04 EEPROM device address bits: 1 0 1 0 A2 A1 P0 R/W
	// A2 A1 set physically to 0 on appriopriate pins
	// P0 – the highest address bit
	i2cSend(eeprom_addr | ((command_address & 0x100) >> 7));
	i2cCheck(0x18, "I2C EEPROM write request")
	i2cSend(command_address & 0xff);
	i2cCheck(0x28, "I2C EEPROM set address")
	i2cStart();
	i2cCheck(0x10, "I2C second start")
	i2cSend(eeprom_addr | 0x1 | ((command_address & 0x100) >> 7));
	i2cCheck(0x40, "I2C EEPROM read request")
	data = i2cReadNoAck();
	i2cStop();
	i2cCheck(0xf8, "I2C stop")
	printf("Read data: addr[0x%.3x] = 0x%x\r\n", command_address, data);
	break;
      }
    }

    else if(0 == strcmp(command_buf, "write"))
    {
      scanf(" %"SCNx8, &data); 
      while(1)
      {
	printf("Assigning addr[0x%.3"PRIx16"] := 0x%"PRIx8"\r\n", command_address, data);
	i2cStart();
	i2cCheck(0x08, "I2C start in write")
	i2cSend(eeprom_addr | ((command_address & 0x100) >> 7));
	i2cCheck(0x18, "I2C EEPROM write request in write")
	i2cSend(command_address & 0xff);
	i2cCheck(0x28, "I2C EEPROM set address in write")
	i2cSend(data);
	i2cCheck(0x28, "I2C EEPROM write")
	i2cStop(); // po 0x28 jest stop?
	i2cCheck(0xf8, "I2C stop")
	break;
      }
    }
    printf("\r\n");

  }
}

