#include <avr/io.h>
#include <util/delay.h>

#define LED PD0
#define LED_FIRST LED
#define LED_LAST PD7
#define LED_DDR DDRD
#define LED_PORT PORTD

int main()
{
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0); // porty szeregowe
	LED_DDR |= 0xFF;
	LED_PORT = 0;
	while (1)
	{

		for(int led_pin = 7; 0 <= led_pin; led_pin--)
		{
			LED_PORT |= _BV(led_pin);
			_delay_ms(50);
			LED_PORT &= ~_BV(led_pin);
		}
		for(int led_pin = 0; led_pin <= 7; led_pin++)
		{
			LED_PORT |= _BV(led_pin);
			_delay_ms(500);
			LED_PORT &= ~_BV(led_pin);
		}
	}
}
