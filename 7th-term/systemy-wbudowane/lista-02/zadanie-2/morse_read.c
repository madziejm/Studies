#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

#define WAIT_MS 20

#define DOT_MS 40
// #define DASH_MS (3 * DOT_MS)
#define DASH_MS 300
#define CHARACTER_SPACE_MS 500
#define WORD_SPACE_MS 1500

// #define UNIT_MS 200

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define MAX_SYMBOLS (8 - 3)
uint8_t digit_codes[10] = {
	// 0bXXX..... - character count
	// 0b...XXXXX - symbols where 0 stands for dot and 1 stands for dash (from right to left)
	0b10111111,
	0b10111110,
	0b10111100,
	0b10111000,
	0b10110000,
	0b10100000,
	0b10100001,
	0b10100011,
	0b10100111,
	0b10101111
};

uint8_t letter_codes['Z' - 'A' + 1] = {
	0b01000010, // a
	0b10000001, // b
	0b10000101, // c
	0b01100001, // d
	0b00100000, // e 
	0b10000100, // f
	0b01100011, // g
	0b10000000, // h
	0b01000000, // i
	0b10001110, // j
	0b01100101, // k
	0b10000010, // l
	0b01000011, // m
	0b01000001, // n
	0b01100111, // o
	0b10000110, // p
	0b10001011, // q
	0b01100010, // r
	0b01100000, // s
	0b00100001, // t
	0b01100100, // u
	0b10001000, // v
	0b01100110, // w
	0b10001001, // x
	0b10001101, // y
	0b10000011, // z 
};

// inicjalizacja UART
void uart_init()
{
	// ustaw baudrate
	UBRR0 = UBRR_VALUE;
	// wyczyść rejestr UCSR0A
	UCSR0A = 0;
	// włącz odbiornik i nadajnik
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	// ustaw format 8n1
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;

void initialize_input_output()
{
	uart_init();
	fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
	stdin = stdout = stderr = &uart_file;
}

int main() {
	BTN_PORT |= _BV(BTN);
	LED_DDR |= _BV(LED);
	LED_PORT &= ~_BV(LED);
	bool push_span = false;
	uint16_t span_time = 0;
	initialize_input_output();

	/* while(1)
	{
		printf("%d\n", BTN_PIN & _BV(BTN));
		_delay_ms(WAIT_MS);
	} */

	uint8_t read_code = 0;
	uint8_t read_signal_count = 0;
	while(1)
	{
		if(BTN_PIN & _BV(BTN))
		{
#ifdef DEBUG
			printf("unpushed\r\n");
#endif
			if(!push_span)
			{
				if(WORD_SPACE_MS <= span_time && span_time <= WORD_SPACE_MS + WAIT_MS)
				{
					putchar(' ');
				}
				else if(read_code && CHARACTER_SPACE_MS <= span_time)
				{
					bool found = false;
					for(uint8_t i = 0; i < sizeof(letter_codes) / sizeof(uint8_t); i++)
						if(letter_codes[i] == read_code)
						{
#ifdef DEBUG
							printf("Found letter %c\r\n", 'a' + i);
#else
							putchar('a' + i);
#endif
							found = true;
							break;
						}
					if(!found)
						for(uint8_t i = 0; i < 10; i++)
							if(digit_codes[i] == read_code)
							{
#ifdef DEBUG
								printf("Found digit %c\r\n", '0' + i);
#else
								putchar('0' + i);
#endif
								found = true;
								break;
							}
					if(!found)
						putchar('?');
					read_code = read_signal_count = 0;
				}
			} else
			{
				LED_PORT &= ~_BV(LED);
#ifdef DEBUG
				printf("pushed -> unpushed\n\r\n");
#endif
				if(DASH_MS <= span_time)
				{
					read_code += 1 << (8 - 3);
					read_code |= 1 << (read_signal_count++);
#ifdef DEBUG
					printf("dash\r\n");
#endif

				}
				else if(DOT_MS <= span_time)
				{
					read_code += 1 << (8 - 3);
					read_signal_count++;
#ifdef DEBUG
					printf("dot\r\n");
#endif
				}
				if(5 < read_signal_count)
				{
					putchar('?');
					read_code = read_signal_count = 0;
				}

				push_span = false;
				span_time = 0;
			}
		}
		else
		{
#ifdef DEBUG
			printf("pushed\r\n");
#endif
			if(push_span)
			{
				if(DASH_MS <= span_time)
					LED_PORT |= _BV(LED);
			}
			else
			{
#ifdef DEBUG
				printf("unpushed -> pushed\n\r\n");
#endif
				push_span = true;
				span_time = 0;
			}

		}
		span_time += WAIT_MS;
		_delay_ms(WAIT_MS);
	}
}
