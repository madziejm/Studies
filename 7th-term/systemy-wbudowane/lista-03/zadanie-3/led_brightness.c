#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define LED PB5
#define LED_PORT PORTB
#define LED_DDR DDRB

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
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

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;

// #define LED_EMIT_LIGHT(lit_step, put_off_step, delay) \
  for(uint16_t i = 0; i < 1000 * (delay) / ((lit_step) + (put_off_step)); i++) \
  { \
    LED_PORT |= _BV(LED); \
    delay_us(lit_step); \
    LED_PORT &= ~_BV(LED); \
    delay_us(put_off_step); \
  }

static const uint8_t exp_vals[33]=
{
0,
1,
1,
1,
1,
2,
2,
3,
3,
4,
5,
6,
7,
9,
11,
13,
16,
19,
23,
28,
34,
41,
49,
59,
71,
85,
102,
122,
147,
177,
212,
255,
};

int main()
{
  LED_DDR |= _BV(LED);
  LED_PORT &= ~_BV(LED);
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  // fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  // stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // mierz napięcie
  while(1) {
    ADCSRA |= _BV(ADSC); // wykonaj konwersję
    while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
    ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    uint32_t delay = 32768; // ms
    uint8_t exponent = v / 32;
    uint8_t lit_step = exp_vals[exponent];
    uint8_t put_off_step = lit_step? -lit_step : 255;
    // printf("Odczytano: %"PRIu16"\r\n", v);
    // printf("%"PRIu16" %"PRIu16"\r\n", lit_step, put_off_step);
    for(uint32_t i = 0; i < delay / 256; i++)
    {
      if(exponent)
      {
        LED_PORT |= _BV(LED);
        _delay_us(lit_step);
      }
      LED_PORT &= ~_BV(LED);
      _delay_us(put_off_step);
    }
  }
}

