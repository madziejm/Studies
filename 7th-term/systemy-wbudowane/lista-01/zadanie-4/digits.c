#include <avr/io.h>
#include <util/delay.h>

#define DIS_DDR DDRD
#define DIS_PORT PORTD

#define SEG_A PD0
#define SEG_B PD1
#define SEG_C PD2
#define SEG_D PD3
#define SEG_E PD4
#define SEG_F PD5
#define SEG_G PD6
#define SEG_DP PD7

int main()
{
	UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0); // porty szeregowe
	DIS_DDR |= 0xFF;
	DIS_PORT = 0;
	_delay_ms(1000);
	int digit_to_port_data[10] = 
	{
		_BV(SEG_G) | _BV(SEG_DP),
		_BV(SEG_A) | _BV(SEG_D) | _BV(SEG_E) | _BV(SEG_F) | _BV(SEG_G) | _BV(SEG_DP),
		_BV(SEG_C) | _BV(SEG_F) | _BV(SEG_DP),
		_BV(SEG_F) | _BV(SEG_E) |  _BV(SEG_DP),
	       	_BV(SEG_A) | _BV(SEG_E) | _BV(SEG_D) | _BV(SEG_DP),
		_BV(SEG_B) | _BV(SEG_E) | _BV(SEG_DP),
		_BV(SEG_B) | _BV(SEG_DP),
		_BV(SEG_G) | _BV(SEG_E) | _BV(SEG_D) | _BV(SEG_DP),
		_BV(SEG_DP),
		_BV(SEG_E) | _BV(SEG_DP),
	};
	while (1)
	{

		for(unsigned digit = 0; digit < 10; digit++)
		{
			DIS_PORT = digit_to_port_data[digit];
			// LED_PORT |= _BV(led_pin);
			_delay_ms(1234);
			//LED_PORT &= ~_BV(led_pin);
			// _delay_ms(50);
		}
	}
}
