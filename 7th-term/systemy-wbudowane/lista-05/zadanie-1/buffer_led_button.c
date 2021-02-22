#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PD2 // INT0
#define BTN_PIN PIND // do not confuse with PORTD
#define BTN_PORT PORTD

#define BUFFER_CELL_COUNT 16
#define BUFFER_CELL_BIT_COUNT 8
#define BUFFER_ELEMENT_COUNT ((BUFFER_CELL_BIT_COUNT) * (BUFFER_CELL_COUNT))
uint8_t push_history_buf[BUFFER_CELL_COUNT]; // zero-initialised
uint8_t start;
uint8_t end;

ISR(TIMER2_COMPA_vect) {
  if(push_history_buf[start / BUFFER_CELL_BIT_COUNT] & (1 << (start % BUFFER_CELL_BIT_COUNT)))
    LED_PORT |= _BV(LED);
  else
    LED_PORT &= ~_BV(LED);
  start++;
  end++;
  start %= BUFFER_ELEMENT_COUNT;
  end %= BUFFER_ELEMENT_COUNT;
  if(BTN_PIN & _BV(BTN))
    push_history_buf[end / BUFFER_CELL_BIT_COUNT] &= ~(1 << (end % BUFFER_CELL_BIT_COUNT));
  else
    push_history_buf[end / BUFFER_CELL_BIT_COUNT] |= 1 << (end % BUFFER_CELL_BIT_COUNT);
}

void set_timer_interrupt()
{
  // set timer 2
  // frequency = 16e6/(1024*(121+1)) = 128 Hz
  TCCR2A = _BV(WGM21) | _BV(WGM20); // fast PWM, top = OCR2A
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20) | _BV(WGM22); // prescaler 1024
  TIMSK2 |= _BV(OCIE2A); // interrupt on Output Compare Flag 2 A
  OCR2A = 121; // Output Compare Flag 2 A
}

int main() {
  BTN_PORT |= _BV(BTN); // set pull up
  LED_DDR |= _BV(LED);
  LED_PORT &= ~_BV(LED);
  start = 0;
  end = BUFFER_ELEMENT_COUNT - 1;
  set_timer_interrupt();
  set_sleep_mode(SLEEP_MODE_IDLE); // type of the following sleep(s)
  sei(); // set global interrupt mask, switches interrupts on
  while (1)
  {
    sleep_mode(); // put into sleep, sets and clears Sleep Enable bit
  }
}
