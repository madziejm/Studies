#include <avr/io.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define R_LED PD6 // OC0A
#define G_LED PB1 // OC1A
#define B_LED PB3 // OC2(A)

#define R_LED_PORT PORTD
#define G_LED_PORT PORTB
#define B_LED_PORT PORTB

#define R_LED_DDR DDRD
#define G_LED_DDR DDRB
#define B_LED_DDR DDRB

#define R_LED_OCR OCR0A
#define G_LED_OCR OCR1A
#define B_LED_OCR OCR2A

void timer_0_init()
{
  // ustaw tryb licznika
  // częstotliwość 16e6/8/256 = 7813 Hz
  // datasheet str. 105
  TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0A0); // set OC0A on compare, fast PWM
  TCCR0B = _BV(CS01); // prescaler 8
}

void timer_1_init()
{
  // ustaw tryb licznika analogicznie na wyjście OC1A
  // non-inverting mode
  // fast PWM top=255
  TCCR1A = _BV(WGM12) | _BV(WGM10) | _BV(COM1A1) | _BV(COM1A0); // set OC1A on compare, fast PWM
  TCCR1B =_BV(CS11);
}

void timer_2_init()
{
  // ustaw tryb licznika analogicznie na wyjście OC2A
  // datasheet str. 155 str. 149
  TCCR2A = _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2A0); // set OC2A on compare match, fast PWM
  TCCR2B = _BV(CS21); // prescaler 8
}

uint8_t sine[64] =
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  2,
  2,
  3,
  4,
  5,
  6,
  7,
  9,
  10,
  12,
  14,
  17,
  19,
  22,
  26,
  29,
  33,
  37,
  42,
  47,
  52,
  58,
  64,
  71,
  77,
  85,
  92,
  100,
  108,
  117,
  125,
  134,
  143,
  152,
  162,
  171,
  180,
  189,
  198,
  206,
  214,
  222,
  229,
  235,
  241,
  246,
  250,
  253,
  254,
  255,
};

int main()
{
  R_LED_DDR |= _BV(R_LED);
  G_LED_DDR |= _BV(G_LED);
  B_LED_DDR |= _BV(B_LED);
  R_LED_PORT |= _BV(R_LED); // common anode -> high pin === LED off
  G_LED_PORT |= _BV(G_LED); // common anode -> high pin === LED off
  B_LED_PORT |= _BV(B_LED); // common anode -> high pin === LED off
  timer_0_init();
  timer_1_init();
  timer_2_init();
  R_LED_OCR = G_LED_OCR = B_LED_OCR = 0;
  while(1) {
    float h = (float)rand() / (float)RAND_MAX,
	  s = 0.9,
	  v = 0.90;
    float r, g, b;
    int h_i = h * 6.0;
    float f = h * 6.0 - h_i,
	  p = v * (1.0 - s),
	  q = v * (1.0 - f * s),
	  t = v * (1.0 - (1.0 - f) * s);
    switch(h_i)
    {
      case 0:
	r = v; g = t; b = p;
	break;
      case 1:
	r = q; g = v; b = p;
	break;
      case 2:
	r = p; g = v; b = t;
	break;
      case 3:
	r = p; g = q; b = v;
	break;
      case 4:
	r = t; g = p; b = v;
	break;
      case 5:
	r = v; g = p; b = q;
	break;
    }
    r *= r;
    g *= g;
    b *= b;
    _delay_ms(200);
    for(int8_t i = 0; i < 64; i++)
    {
      R_LED_OCR = sine[i] * r;
      G_LED_OCR = sine[i] * g;
      B_LED_OCR = sine[i] * b;
      _delay_ms(20);
    }
    for(int8_t i = 63; 0 <= i; i--)
    {
      R_LED_OCR = sine[i] * r;
      G_LED_OCR = sine[i] * g;
      B_LED_OCR = sine[i] * b;
      _delay_ms(20);
    }
    _delay_ms(500);
  }
}

