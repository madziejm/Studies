#include <avr/io.h>
#include <util/delay.h>

#define DIS_DDR DDRD
#define DIS_PORT PORTD

#define DIS_TRANSISTORS_DDR DDRC
#define DIS_TRANSISTORS_PORT PORTC
#define DIS_FST_DGT_TRANSISTOR PC1
#define DIS_SND_DGT_TRANSISTOR PC0

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
	DIS_TRANSISTORS_DDR |= (_BV(DIS_FST_DGT_TRANSISTOR) | _BV(DIS_SND_DGT_TRANSISTOR));
	DIS_PORT = 0;
	DIS_TRANSISTORS_PORT &= ~(_BV(DIS_FST_DGT_TRANSISTOR) | ~_BV(DIS_SND_DGT_TRANSISTOR));
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

		for(unsigned fst_digit = 0; fst_digit < 6; fst_digit++)
		{
			for(unsigned snd_digit = 0; snd_digit < 10; snd_digit++)
			{
				for(unsigned time_ms = 0; time_ms < 1000;)
				{
					DIS_TRANSISTORS_PORT |= _BV(DIS_FST_DGT_TRANSISTOR);
					DIS_PORT = digit_to_port_data[fst_digit];
					_delay_ms(10);
					time_ms += 10;
					DIS_TRANSISTORS_PORT &= ~_BV(DIS_FST_DGT_TRANSISTOR);
					DIS_TRANSISTORS_PORT |= _BV(DIS_SND_DGT_TRANSISTOR);
					DIS_PORT = digit_to_port_data[snd_digit];
					_delay_ms(10);
					time_ms += 10;
					DIS_TRANSISTORS_PORT &= ~_BV(DIS_SND_DGT_TRANSISTOR);
				}
			}
		}
	}
}
