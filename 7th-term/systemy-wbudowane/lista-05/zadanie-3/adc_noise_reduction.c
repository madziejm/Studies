#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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
  // ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  ADMUX = _BV(REFS0) | 0b1110; // nap. referencyjne zasilające, wejście analogowe na nap. ref. 1,1 V
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  const uint8_t reading_count = 8;
  float readings[reading_count];
  while(1) {
    // no ADC Noise Reduction
    printf("no noise reduction:\n\r");
    float reading_sum = 0.0;
    for(uint8_t i = 0; i < reading_count; i++)
    {
      ADCSRA |= _BV(ADSC); // wykonaj konwersję
      while (!(ADCSRA & _BV(ADIF))); // czekaj na wynik
      ADCSRA |= _BV(ADIF); // wyczyść bit ADIF (pisząc 1!)
      uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
      float vcc = 1.1 * 1023.0 / (float)v;
      printf("odczytano z ADC: %"PRIu16" %"PRIu16"\n\r", v, i);
      printf("nap.: %f\n\r", vcc);
      readings[i] = vcc;
      reading_sum += vcc;
    }
    float reading_average = reading_sum / (float)reading_count;
    float variance = 0.0;
    for(uint8_t i = 0; i < reading_count; i++)
      variance += (readings[i] - reading_average) * (readings[i] - reading_average);
    variance /= (float)reading_count;
    printf("wariancja: %f\n\r", variance);
    printf("\n");
    _delay_ms(500);

    // ADC Noise Reduction
    printf("noise reduction:\n\r");
    reading_sum = 0.0;
    variance = 0.0;
    ADCSRA |= _BV(ADIE) | _BV(ADEN); // ADIE -- generate an interrupt when measurement is done
    set_sleep_mode(SLEEP_MODE_ADC); // set SMCR (Sleep Mode Control Register)

    for(uint8_t i = 0; i < 8; i++)
    {
      sei();
      // sleep_mode();
      sleep_cpu();
      uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
      float vcc = 1.1 * 1023.0 / (float)v;
      printf("odczytano z ADC: %"PRIu16" %"PRIu16"\n\r", v, i);
      printf("nap.: %f\n\r", vcc);
      readings[i] = vcc;
      reading_sum += vcc;
    }
    reading_average = reading_sum / (float)reading_count;
    for(uint8_t i = 0; i < 8; i++)
      variance += (readings[i] - reading_average) * (readings[i] - reading_average);
    variance /= (float)reading_count;
    printf("wariancja: %f\n\r", variance);

    printf("\n\n\r");
    _delay_ms(5000);
  }
}

