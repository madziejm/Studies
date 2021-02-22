#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
// #include <avr/pgmspace.h>

#include <util/delay.h>

#include "sound.h"

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem


#define PORT_SPI PORTB
#define DDR_SPI  DDRB
#define MISO     PB4
#define MOSI     PB3
#define SS       PB2
#define SCK      PB5


void SPI_master_init()
{
  // Set MOSI and SCK and SS output, all others input
  DDR_SPI = _BV(MOSI) | _BV(SCK) | _BV(SS);
  // Enable SPI, Master, set clock rate fck/64
  // CPOL=0 CPHA=0 SPI mode
  // SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1); // clock /2
}

void timer1_interrupt_init()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // 8000 Hz (16000000/((1+1)*1))
  OCR1A = 249;
  TCCR1B |= _BV(WGM12);
  // Prescaler 1
  TCCR1B |= _BV(CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= _BV(OCIE1A);
  sei();
}

void SPI_master_transmit(char byte)
{
  SPDR = byte;
  while(!(SPSR & _BV(SPIF))); // wait for transmission complete 
}


int main()
{
  SPI_master_init();
  // timer1_interrupt_init();
  // set_sleep_mode(SLEEP_MODE_IDLE);
  uint8_t data;
  // sei();  
  while(1)
  {
    // sleep_mode();
    for(uint16_t i = 0; i < sound_pcm_len; i++)
    {
      data = pgm_read_byte(&sound_pcm[i]);
      uint8_t control_bits = 0b00110000; // unbuffered, output gain 1
      uint16_t duplet = control_bits << 8 | ((uint16_t)data << 4);
      PORT_SPI &= ~_BV(SS);
      SPI_master_transmit(duplet >> 8);   // most significant  byte
      SPI_master_transmit(duplet & 0xff); // least significant byte
      PORT_SPI |= _BV(SS);
      _delay_us(10);
      // sleep_mode();
    }
  }
}

