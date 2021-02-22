#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <inttypes.h>

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

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();
  // program testowy
  while(1) {
    volatile uint16_t time_a, time_b;
    {
      printf("uint8_t\r\n");
      uint8_t a, b;
      uint8_t c;
      scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a + b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu8"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a - b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu8"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a * b;
      time_b = TCNT1; // wartość licznika po czekaniu
      //printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu8"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a / b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu8"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a % b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu8"\r\n", time_b - time_a, c);
      puts("");
    }
    {
      printf("uint16_t\r\n");
      uint16_t a, b;
      uint16_t c;
      scanf("%" SCNd16 "%" SCNd16, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a + b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu16"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a - b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu16"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a * b;
      time_b = TCNT1; // wartość licznika po czekaniu
      //printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu16"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a / b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu16"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a % b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu16"\r\n", time_b - time_a, c);
      puts("");
    }
    {
      printf("uint32_t\r\n");
      uint32_t a, b;
      uint32_t c;
      scanf("%" SCNd32 "%" SCNd32, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a + b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a - b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a * b;
      time_b = TCNT1; // wartość licznika po czekaniu
      //printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a / b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, c);
      // scanf("%" SCNd8 "%" SCNd8, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a % b;
      time_b = TCNT1; // wartość licznika po czekaniu
      // printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, c);
      puts("");
    }
    {
      printf("uint64_t\r\n");
      uint64_t a, b;
      uint64_t c;
      scanf("%" SCNd32 "%" SCNd32, &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a + b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, (uint32_t)c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a - b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, (uint32_t)c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a * b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, (uint32_t)c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a / b;
	// aaa
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, (uint32_t)c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a % b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %"PRIu32"\r\n", time_b - time_a, (uint32_t)c);
      puts("");
    }
    {
      printf("float\r\n");
      float a, b;
      float c;
      scanf("%f %f", &a, &b);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a + b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %f\r\n", time_b - time_a, c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a - b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %f\r\n", time_b - time_a, c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a * b;
      time_b = TCNT1; // wartość licznika po czekaniu
      //printf("Zmierzony czas: %"PRIu16" cykli\r\n", time_b - time_a);
      printf("Zmierzony czas: %"PRIu16" cykli %f\r\n", time_b - time_a, c);
      time_a = TCNT1; // wartość licznika przed czekaniem
	c = a / b;
      time_b = TCNT1; // wartość licznika po czekaniu
      printf("Zmierzony czas: %"PRIu16" cykli %f\r\n", time_b - time_a, c);
      puts("");
    }
    _delay_ms(1000);
  }
}

