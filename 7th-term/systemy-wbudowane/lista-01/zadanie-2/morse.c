#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <ctype.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define UNIT_MS 200

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

void led_switch_on()
{
    LED_PORT |= _BV(LED);
}

void led_switch_off()
{
    LED_PORT &= ~_BV(LED);
}

void transmit_morse_dot()
{
	unsigned dot_length = UNIT_MS;
	led_switch_on();
	_delay_ms(dot_length);
	led_switch_off();
}

void transmit_morse_dash()
{
	unsigned dash_length = UNIT_MS * 3;
	led_switch_on();
	_delay_ms(dash_length);
	led_switch_off();
}

const char* get_morse_code(char c)
{
	char* letter_codes['Z' - 'A' + 1] =
	{
		"*-",
		"-***",
		"-*-*",
		"-**",
		"*",
		"**-*",
		"--*",
		"****",
		"**",
		"*---",
		"-*-",
		"*-**",
		"--",
		"-*",
		"---",
		"*--*",
		"--*-",
		"*-*",
		"***",
		"-",
		"**-",
		"***-",
		"*--",
		"-**-",
		"-*--",
		"--**",
	};
	char* digit_codes['9' - '0' + 1] =
	{
		"-----",
		"*----",
		"**---",
		"***--",
		"****-",
		"*****",
		"-****",
		"--***",
		"---**",
		"----*",
	};
	if(isdigit(c))
		return digit_codes[c - '0'];
	if(islower(c))
		return letter_codes[c - 'a'];
	if(isupper(c))
		return letter_codes[c - 'A'];
	return "";
}

void transmit_morse_character(const char* character_code)
{
	unsigned inletter_space_delay_ms = UNIT_MS;
	unsigned inword_space_delay_ms = 3 * UNIT_MS;
	while(*character_code)
	{
		if('-' == *character_code)
			transmit_morse_dash();
		if('*' == *character_code)
			transmit_morse_dot();
		character_code++;
		if(*character_code)
			_delay_ms(inletter_space_delay_ms);
	}
	_delay_ms(inword_space_delay_ms);
}

void morse_print(const char* string)
{
	unsigned insentence_space_delay_ms = 7 * UNIT_MS;
	while(*string)
	{
		if(isalnum(*string))
		{
			transmit_morse_character(get_morse_code(*string));
		}
		if(isspace(*string))
			_delay_ms(insentence_space_delay_ms);
		string++;

	}
}



int main() {
  LED_DDR |= _BV(LED);
  initialize_input_output();
  while (1) {
	  puts("Put message: ");
	  char message[128];
	  scanf("%128s", message);
	  // scanf("%128[^\n]\n", message);
	  // gets(message);
	  puts(message);
	  putchar('\r');
	  morse_print(message);
    // _delay_ms(1000);
    // _delay_ms(1000);
  }
}
