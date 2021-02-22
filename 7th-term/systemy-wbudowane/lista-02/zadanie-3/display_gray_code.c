#include <avr/io.h>
#include <util/delay.h>

// #define FST_LED PD0
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB


int main() {
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0); // porty szeregowe
	BTN_PORT |= _BV(BTN);
	LED_DDR |= 0xFF;
	LED_PORT &= ~0xFF;
	uint8_t number = 0;
	while(1)
	{
		if(!(BTN_PIN & _BV(BTN)))
		{
			number++;
			uint8_t gray_code = number ^ (number >> 1);
			LED_PORT &= ~0xFF;
			LED_PORT |= 0xFF & gray_code;
			while(!(BTN_PIN & _BV(BTN)))
				_delay_ms(10);
		}
	}
}
