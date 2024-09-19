#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

// DEFINICIONES DE BAUT RATE
#define F_CPU 1000000UL         // Frecuencia del CPU 
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// DEFINICIONES DE ESTADOS
#define standby 0
#define start 1         // Define del estado de inicio
#define secuence 2        // Define del estado que genera la suecuencia
#define user 3            // Define del estado que gestiona el reinicio del juego

void uart_init(unsigned int ubrr);
void uart_transmit(unsigned char data);
int uart_putchar(char c, FILE *stream);

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

int main(){    
/******************************************************************************
 * PARA UART 
 *****************************************************************************/

  // Inicializar UART con el baud rate definido
  uart_init(MYUBRR);
  // Redirigir el standar output hacia UART
  stdout = &uart_output;

/******************************************************************************
 * Inicio del codigo
 *****************************************************************************/

  // Configuración de puertos B y D como entrada/salida
  DDRB = 0x0F; // Configura los pines 0,1,2 y 3 del puerto B como salida
  DDRD = 0x00; // Configura todos los pines del puerto D como entrada
  
  int state = standby;                    // Variable de estado
  int counterSecuence = 4;                // Variable para controlar el tamaño de la secuencia
  int initSec = 0;
  int secuenceLed[13] = {0};              // Array para guardar la secuencia
  int buttos[4] = {1,2,4,8};              // Numeros representativos de los botones
  int randNum; 
  int bPressed = 0;
  int bRed = 0;
  int bYellow = 0;
  int bGreen = 0;
  int bBlue = 0;

  TCCR0B |= (1 << CS01); // Configurar el Timer0 con prescaler de 8
  TCNT0 = 0; // Inicia el contador en 0

  sei();

  while (1){
    _delay_ms(1000);

    bRed =    (PIND & (1 << PD0)); // Entrada PD0 como botón 0
    bGreen =  (PIND & (1 << PD1)); // Entrada PD1 como botón 1
    bYellow = (PIND & (1 << PD2)); // Entrada PD2 como botón 2
    bBlue =   (PIND & (1 << PD3)); // Entrada PD3 como botón 3


    switch (state){
      case standby:
        if (bRed || bGreen || bYellow || bBlue){
          state = start;
          }
      break;

      case start:

        if (counterSecuence > 13){        // Si el tamaño de la secuencia supera 13 colores debe reiniciarse
          counterSecuence = 4;
        }

        if (counterSecuence == 4){
          initSec = 0;
        } else {
          initSec = counterSecuence;
        }
            state = secuence;
            // Ciclo for donde se genera la secuencia inicial
            for (int i = 0; i<= counterSecuence - 1; i++){
                randNum = buttos[TCNT0 % 4];
                printf("\nnumero aleatorio es: %d ", randNum);
                _delay_ms(100);
                secuenceLed[i] = randNum;
            }
      break;

      case secuence:
        state = user;
        _delay_ms(10000);
        for (int i = 0; i<= counterSecuence -1; i++){
          bPressed = 0;

        switch (secuenceLed[i]) {
        case 1:
            while (!bPressed) {
                if (PIND & (1 << PD0)) {
                    bPressed = 1;
                    printf("Botón Rojo\n");
                }
            }
            break;

        case 2:
            while (!bPressed) {
                if (PIND & (1 << PD1)) {
                    bPressed = 1;
                    printf("Botón Verde\n");
                }
            }
            break;

        case 4:
            while (!bPressed) {
                if (PIND & (1 << PD2)) {
                    bPressed = 1;
                    printf("Botón Amarillo\n");
                }
            }
            break;

        case 8:
            while (!bPressed) {
                if (PIND & (1 << PD3)) {
                    bPressed = 1;
                    printf("Botón Azul\n");
                }
            }
            break;

        default:
            printf("Error: Valor inesperado en secuencia\n");
            break;
        }
        }
        counterSecuence++;
      break;

      case user:
        state = start;
        printf("\nestado que no sirve");
      break;
      default:
        printf("Caso default");
      break;
    }
  }
}

// Inicializa UART
void uart_init(unsigned int ubrr) {
  // Configura el baud rate
  UBRRH = (unsigned char)(ubrr >> 8);
  UBRRL = (unsigned char)ubrr;
  
  // Habilita transmisión
  UCSRB = (1 << TXEN);
  
  // Configura el formato del marco de datos: 8 bits de datos, 1 bit de parada
  UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
}

// Envía un byte a través de UART
void uart_transmit(unsigned char data) {
  // Espera hasta que el buffer de transmisión esté vacío
  while (!(UCSRA & (1 << UDRE)));
  // Coloca el dato en el registro de transmisión
  UDR = data;
}

// Función para redirigir stdout a UART
int uart_putchar(char c, FILE *stream) {
  if (c == '\n') {
    uart_transmit('\r'); // Carriage return para nueva línea en terminales
  }
  uart_transmit(c);
  return 0;
}