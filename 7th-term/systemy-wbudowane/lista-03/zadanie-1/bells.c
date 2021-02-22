#include <avr/pgmspace.h>
#include <avr/io.h>
// #define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <inttypes.h>

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

void delay_us(uint16_t ms)
{
  while(ms--)
    _delay_us(1);
}

#define TONE(step, delay) \
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      delay_us(step); \
    }

void tone(uint16_t step, uint16_t delay)
{
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step) / 2; i++) { \
      BUZZ_PORT |= _BV(BUZZ); \
      delay_us(step); \
      BUZZ_PORT &= ~_BV(BUZZ); \
      delay_us(step); \
    }
}

typedef uint8_t sound;
// bits 0..1 for note time duration
// bits 2..4 for note pitch
// bits 5..7 for octave

#define WHOLE_NOTE_MS 1333

#define WHOLE_NOTE     0x0
#define HALFNOTE       0x1
#define QUARTER_NOTE   0x2
#define EIGHTH_NOTE    0x3
#define SIXTEENTH_NOTE 0x4

#define C_NOTE      0x0
#define D_NOTE      0x1
#define E_NOTE      0x2
#define F_NOTE      0x3
#define G_NOTE      0x4
#define A_NOTE      0x5
#define B_NOTE      0x6
#define H_NOTE B_NOTE
#define SILENT_NOTE 0x7

#define C1_OCTAVE 0x0
#define C2_OCTAVE 0x1
#define C3_OCTAVE 0x2
#define C4_OCTAVE 0x3
#define C5_OCTAVE 0x4
#define C6_OCTAVE 0x5
#define C7_OCTAVE 0x6
#define C8_OCTAVE 0x7

#define LAST_SOUND 0xFF

uint16_t note_frequencies[] = // in fact these are not frequencies
{
  (1000000.0/261.1),
  (1000000.0/293.7),
  (1000000.0/329.6),
  (1000000.0/349.6),
  (1000000.0/391.9),
  (1000000.0/440.0),
  (1000000.0/493.9)
};

// static const sound song[] PROGMEM = 
// {
//   E_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   A_NOTE << 2 | EIGHTH_NOTE,
//   A_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   C_NOTE << 2 | HALFNOTE,
//   E_NOTE << 2 | QUARTER_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | QUARTER_NOTE,
//   E_NOTE << 2 | HALFNOTE,
//   D_NOTE << 2 | QUARTER_NOTE,
//   D_NOTE << 2 | EIGHTH_NOTE,
//   D_NOTE << 2 | QUARTER_NOTE,
//   D_NOTE << 2 | HALFNOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   A_NOTE << 2 | EIGHTH_NOTE,
//   A_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   E_NOTE << 2 | EIGHTH_NOTE,
//   C_NOTE << 2 | QUARTER_NOTE,
//   E_NOTE << 2 | QUARTER_NOTE,
//   LAST_SOUND,
// };

/*
static const sound song[] PROGMEM = 
{
  C1_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C1_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C2_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C2_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C3_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C4_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C4_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C5_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,
 
  SILENT_NOTE << 2 | WHOLE_NOTE,

  C6_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C6_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C7_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C7_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,

  SILENT_NOTE << 2 | WHOLE_NOTE,

  C8_OCTAVE << 5 | C_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | D_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | E_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | F_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | G_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C8_OCTAVE << 5 | B_NOTE << 2 | WHOLE_NOTE,
  
  SILENT_NOTE << 2 | WHOLE_NOTE,
  
  LAST_SOUND,
};
*/


static const sound song[] PROGMEM = 
{
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  // 0:14 left hand
  C2_OCTAVE << 5 | A_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | HALFNOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | HALFNOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | HALFNOTE,
  SILENT_NOTE << 2 | QUARTER_NOTE,
  C3_OCTAVE << 5 | G_NOTE << 2 | HALFNOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | QUARTER_NOTE,
  C3_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | HALFNOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | HALFNOTE,
  // C2_OCTAVE << 5 | A_NOTE << 2 | WHOLE_NOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C3_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | HALFNOTE,
  SILENT_NOTE << 2 | QUARTER_NOTE,
  C3_OCTAVE << 5 | G_NOTE << 2 | HALFNOTE,
  C3_OCTAVE << 5 | D_NOTE << 2 | QUARTER_NOTE,
  C3_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C2_OCTAVE << 5 | G_NOTE << 2 | QUARTER_NOTE,
  C2_OCTAVE << 5 | A_NOTE << 2 | QUARTER_NOTE,
  // 00:29 
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | G_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | A_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | D_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C6_OCTAVE << 5 | C_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | B_NOTE << 2 | EIGHTH_NOTE,
  C5_OCTAVE << 5 | E_NOTE << 2 | EIGHTH_NOTE,
  LAST_SOUND
};


int main() {
  BUZZ_DDR |= _BV(BUZZ);
  while (1) {
    const sound* song_ptr = song;
    sound song_sound = pgm_read_byte(song_ptr);
    while(LAST_SOUND != song_sound)
    {
      uint16_t note_time_duration_ms = WHOLE_NOTE_MS >> (song_sound & 0b11);
      if(((song_sound >> 2) & 0b111) == SILENT_NOTE)
      {
	delay_us(note_time_duration_ms * 1000);
      }
      else
      {
	uint8_t note_octave = (song_sound >> 5) & 0b111;
	uint16_t frequency = note_frequencies[(song_sound >> 2)  & 0b111] >> note_octave; // next octave -> 2x shorter time
	TONE(frequency, note_time_duration_ms);
      }
      _delay_ms(50);
      song_sound = pgm_read_byte(song_ptr++);
    }
    _delay_ms(5000);
  }
}
