#include <avr/io.h>
#include <util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define BUF_BYTES 16
uint8_t push_history_buf[BUF_BYTES];
uint8_t start;
uint8_t end;

int main() {
	BTN_PORT |= _BV(BTN);
	//LED_DDR |= _BV(LED);
	//LED_DDR |= 0xFF;
	//LED_PORT &= ~0xFF;
	LED_PORT &= ~_BV(LED);
	start = 0;
	end = BUF_BYTES * 8 - 1;
	while (1)
	{
		if(push_history_buf[start / 8] & (1 << (start % 8)))
			LED_PORT |= _BV(LED);
		else
			LED_PORT &= ~_BV(LED);
		start++;
		start %= BUF_BYTES * 8;
		end++;
		end %= BUF_BYTES * 8;
		if(BTN_PIN & _BV(BTN))
			push_history_buf[end / 8] &= ~(1 << (end % 8));
		else
			push_history_buf[end / 8] |= 1 << (end % 8);
		_delay_ms(1000 / (BUF_BYTES * 8));
	}
}
