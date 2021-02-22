#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "i2c.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define READ  1
#define WRITE 0

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

const uint8_t eeprom_addr = 0xa0; // 0b1010...

int main()
{
  uart_init();
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
    char command_buf[strlen("write") + 1];
    scanf("%s ", command_buf);
    printf("Received command %s\r\n", command_buf);
    
    if(0 == strcmp(command_buf, "read"))
    {
      uint16_t read_address;
      uint8_t read_length; // max 0xff
      scanf("%"SCNx16" %"SCNx8, &read_address, &read_length);
      // printf("read_address=0x%.4"PRIx16" read_length=0x%.2"PRIx8"\r\n", read_address, read_length);
      uint8_t checksum = (read_address >> 8) + (uint8_t)(read_address) + read_length;
      printf(":%.2"PRIx8"%.4"PRIx16"00", read_length, read_address);
      
      while(1)
      {
	i2cStart();
	i2cCheck(0x08, "I2C start in read");
	// 4K 24C04 EEPROM device address bits: 1 0 1 0 A2 A1 P0 R/W
	// A2 A1 set physically to 0 on appriopriate pins
	// P0 – the highest address bit
	i2cSend(eeprom_addr | ((read_address & 0x100) >> 7));
	i2cCheck(0x18, "I2C EEPROM write request")
	i2cSend(read_address & 0xff);
	i2cCheck(0x28, "I2C EEPROM set address")
	i2cStart();
	i2cCheck(0x10, "I2C second start")
	i2cSend(eeprom_addr | ((read_address & 0x100) >> 7) | READ);
	i2cCheck(0x40, "I2C EEPROM read request")
	break;
      }
      for(uint8_t i = 0; i < read_length; i++)
      {
	uint8_t read_data;
	if(i == read_length - 1)
	{
	  read_data = i2cReadNoAck();
	  i2cCheck(0x58, "I2C EEPROM read")
	}
	else
	{
	  read_data = i2cReadAck();
	  i2cCheck(0x50, "I2C EEPROM read")
	}
	checksum += read_data;
	printf("%.2"PRIx8, read_data);
      }
      while(1)
      {
	i2cStop();
	i2cCheck(0xf8, "I2C stop")
	// printf("Read data: addr[0x%.3x] = 0x%x\r\n", read_address, read_data);
	break;
      }
      checksum = -checksum;
      printf("%.2"PRIx8"\r\n\r\n", checksum);
    }
    else if(0 == strcmp(command_buf, "write"))
    {
      uint8_t byte_count;
      uint16_t write_address;
      uint8_t record_type, record_checksum;
      while(1) // accept any record until end of file record
      {
	if(':' != getchar())
	  continue;
	uint8_t data_buf[255];
	scanf("%2"SCNx8"%4"SCNx16"%2"SCNx8, &byte_count, &write_address, &record_type);
	uint8_t calculated_checksum = byte_count + (write_address >> 8) + (uint8_t)(write_address) + record_type;
	if(1 == record_type)
	{
	  getchar(); getchar(); // checksum
	  break;
	}
	if(0 != record_type)
	  break;
	for(uint16_t i = 0; i < byte_count; i++)
	{
	  uint8_t data;
	  scanf("%2"SCNx8, &data);
	  printf("read %.2"PRIx8"\r\n", data);
	  data_buf[i] = data;
	  calculated_checksum += data;
	}
	scanf("%2"SCNx8, &record_checksum);
	calculated_checksum = -calculated_checksum;
	if(record_checksum != calculated_checksum)
	{
	  printf("checksum mismatch 0x%"PRIx8" vs 0x%"PRIx8"\r\n", calculated_checksum, record_checksum);
	  continue;
	}
	
	for(uint16_t i = 0; i < byte_count;)
	{
	  while(1)
	  {
	    printf("address=0x%"PRIx16"\r\n", write_address + i);
	    i2cStart();
	    i2cCheck(0x08, "I2C start in write")
	    i2cSend(eeprom_addr | (((write_address + i) & 0x100) >> 7) | WRITE);
	    i2cCheck(0x18, "I2C EEPROM write request in write")
	    i2cSend((write_address + i) & 0xff);
	    i2cCheck(0x28, "I2C EEPROM set address in write")
	    break;
	  }
	  for(uint8_t j = 0; j < 16 && i < byte_count; i++, j++) 
	  {
	    printf("i=0x%"PRIx16" j=0x%"PRIx8"\r\n", i, j);
	    printf("data=0x%"PRIx8"\r\n", data_buf[i]);
	    i2cSend(data_buf[i]);
	    i2cCheck(0x28, "I2C EEPROM write")
	  }
	  while(1)
	  {
	    i2cStop(); // po 0x28 jest stop?
	    i2cCheck(0xf8, "I2C stop")
	    break;
	  }
	}
	printf("record written\r\n");
      }
      printf("write sequence end\r\n\r\n");
    }
  }
}

