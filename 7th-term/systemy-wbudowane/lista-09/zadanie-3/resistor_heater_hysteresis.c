// Lista 9 -- zadanie 3
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define TRANSISTOR      PB5
#define TRANSISTOR_DDR  DDRB
#define TRANSISTOR_PORT PORTB

void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  char received = UDR0;
  uart_transmit(received, NULL);
  return received;
}

void adc_init()
{
  // ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  ADMUX = _BV(REFS0) | _BV(REFS1); // nap. referencyjne 1,1 V, wejście analogowe ADC0
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
  TRANSISTOR_DDR |= _BV(TRANSISTOR);
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();

  float desired_temp;
  printf("Zadana temperatura: ");
  scanf("%f", &desired_temp);
  puts("");
  printf("\r\nZadana temperatura: %f\r\n", desired_temp);
  
  bool heater_on = false;

  while(1) {
   ADCSRA |= _BV(ADSC); // wykonaj konwersję
    
    ADCSRA |= _BV(ADIE) | _BV(ADEN); // ADIE -- generate an interrupt when measurement is done
    set_sleep_mode(SLEEP_MODE_ADC); // set SMCR (Sleep Mode Control Register)
    sei();
    // sleep_mode();
    if(heater_on)
      TRANSISTOR_PORT &= ~_BV(TRANSISTOR);
    sleep_cpu();
    uint16_t v = ADC; // weź zmierzoną wartość (0..1023)
    cli();

    if(heater_on)
      TRANSISTOR_PORT |= _BV(TRANSISTOR);
    
    float VCC = 1.1 / 1023.0 * (float)v;
    const float V_0 = 0.5;
    const float T_C = 0.01;
    float T_A = (VCC - V_0) / T_C;
    float desired_temp_delta = 1.0;

   if(heater_on)
     heater_on = T_A < desired_temp;
   else
     heater_on = T_A < desired_temp - desired_temp_delta;

   // heater_on = true;
   
    if(heater_on)
      TRANSISTOR_PORT |= _BV(TRANSISTOR);
    else
      TRANSISTOR_PORT &= ~_BV(TRANSISTOR);
    printf("odczytano z ADC: %04"PRIu16" nap.: %.5f V; grzalka jest %s; temperatura: %2.3f   \r", v, VCC, heater_on? " wlaczona" : "wylaczona", T_A);
    _delay_ms(500);
  }
}

