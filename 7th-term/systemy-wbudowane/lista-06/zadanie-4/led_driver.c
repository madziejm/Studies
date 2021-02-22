#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <util/delay.h>


#define PORT_SPI PORTB
#define DDR_SPI  DDRB
#define MOSI     PB3
#define SS       PB2
#define SCK      PB5
#define LA       PB1
#define OE       PB2

#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6

void SPI_master_init()
{
  // Set MOSI and SCK and SS output, all others input
  DDR_SPI = _BV(MOSI) | _BV(SCK) | _BV(SS);
  // Enable SPI, Master, set clock rate fck/16
  // CPOL=0 CPHA=0 SPI mode
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
}

void SPI_master_transmit(char byte)
{
  SPDR = byte;
  while(!(SPSR & _BV(SPIF))); // wait for transmission complete 
}

int main()
{
  uint8_t digit_to_data[10] = 
  {
    _BV(SEG_G),
    _BV(SEG_A) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G),
    _BV(SEG_C) | _BV(SEG_F),
    _BV(SEG_F) | _BV(SEG_E),
    _BV(SEG_A) | _BV(SEG_E) | _BV(SEG_D),
    _BV(SEG_B) | _BV(SEG_E),
    _BV(SEG_B),
    _BV(SEG_G) | _BV(SEG_E) | _BV(SEG_D),
    0,
    _BV(SEG_E)
  };
  DDR_SPI |= _BV(LA) | _BV(OE);
  SPI_master_init();
  PORT_SPI &= ~_BV(OE);
  while(1)
  {
    for(uint8_t i = 0; i < 10; i++)
    {
      PORT_SPI |= _BV(LA) | _BV(OE);
      uint8_t data = ~digit_to_data[i];
      PORT_SPI &= ~_BV(SS);
      SPI_master_transmit(data & 0x7f);
      PORT_SPI |= _BV(SS);
      PORT_SPI &= ~_BV(LA) & _BV(OE);
      PORT_SPI &= ~_BV(SS);
      _delay_ms(1000);
    }
  }
}


