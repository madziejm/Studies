// Lista 9 - zadanie 2

#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>
#include "hd44780.h"

int hd44780_transmit(char data, FILE *stream)
{
  LCD_WriteData(data);
  return 0;
}

FILE hd44780_file;

#define SET_CGRAM_ADDR_INSTR 0x40 // 0b 0001 ACG  ACG  ACG  ACG  ACG  ACG (ACG:CGRAM address)
#define FIRST_BLOCK_CHARACTER_CODE 0x80 // 0b1000.0000
#define CGRAM_FIRST_BLOCK_CHARACTER_ADDR 0x80 // 0b1000.0000

#define CHARACTER_HEIGHT 8 

void custom_characters_setup() {
  LCD_WriteCommand(SET_CGRAM_ADDR_INSTR | 0x0);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x00);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x10);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x18);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x1c);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x1e);
  for(int i = 0; i < CHARACTER_HEIGHT; i++)
    LCD_WriteData(0x1f);
}

#define PROGRESSBAR_MAX 80
#define PROGRESSBAR_CHAR_COUNT 16 

int main()
{
  // skonfiguruj wyświetlacz
  LCD_Initialize();
  custom_characters_setup();
  LCD_Clear();
  // skonfiguruj strumienie wyjściowe
  fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
  stdout = stderr = &hd44780_file;
  // program testowy
  while(1) {
    for(uint8_t progress = 0; progress <= PROGRESSBAR_MAX; progress++) {
      LCD_GoTo(0, 0);
      printf("%"PRIu8" of %"PRIu8" ", progress, PROGRESSBAR_MAX);
      LCD_GoTo(0, 1);
      uint8_t progress_remainder = progress;
      for(uint8_t block = 0; block < PROGRESSBAR_CHAR_COUNT; block++)
      {
	putchar(5 < progress_remainder? 5 : progress_remainder);
	if(5 < progress_remainder)
	  progress_remainder -= 5;
	else if(progress_remainder)
	  progress_remainder = 0;
      }
    //_delay_ms(1000);
    _delay_ms(10 * progress);
    }
    _delay_ms(2000);
  }
}

