#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

// DEFINICIONES DE ESTADOS
#define standby 0
#define start 1         // Define del estado de inicio
#define secuence 2        // Define del estado que genera la suecuencia
#define user 3            // Define del estado que gestiona el reinicio del juego

#define ROJO 1
#define AMARI 2
#define VERDE 4
#define AZUL 8

void delayVar(uint16_t delay);

int main(){    
/******************************************************************************
 * Inicio del codigo
 *****************************************************************************/

  // Configuración de puertos B y D como entrada/salida
  DDRB = 0x0F; // Configura los pines 0, 1, 2 y 3 del puerto B como salida
  DDRD = 0x00; // Configura todos los pines del puerto D como entrada
  
  int state = standby;                    // Variable de estado
  int counterSecuence;                // Variable para controlar el tamaño de la secuencia
  int initSec;
  int ledTime;
  int secuenceLed[13] = {0};              // Array para guardar la secuencia
  int buttos[4] = {ROJO, AMARI, VERDE, AZUL};              // Numeros representativos de los botones
  int randNum; 
  int bPressed = 0;
  int bRed = 0;
  int bYellow = 0;
  int bGreen = 0;
  int bBlue = 0;

  TCCR0B |= (1 << CS01); // Configurar el Timer0 con prescaler de 8
  TCNT0 = 0; // Inicia el contador en 0

  sei();

  inicio:
  counterSecuence = 4;
  ledTime = 20000;
  while (1){
    _delay_ms(10000);

    bRed =    (PIND & (1 << PD0)); // Entrada PD0 como botón 0
    bGreen =  (PIND & (1 << PD1)); // Entrada PD1 como botón 1
    bYellow = (PIND & (1 << PD2)); // Entrada PD2 como botón 2
    bBlue =   (PIND & (1 << PD3)); // Entrada PD3 como botón 3
    switch (state){
      case standby:
        PORTB = 0x0F;
        _delay_ms(1000);
        PORTB = 0x00;
        _delay_ms(1000);
        PORTB = 0x0F;
        _delay_ms(1000);
        PORTB = 0x00;
        _delay_ms(1000);
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
            for (int i = initSec; i<= counterSecuence - 1; i++)
            {
                randNum = buttos[TCNT0 % 4];
                  PORTB = 0x00;
                secuenceLed[i] = randNum;

                if (randNum == ROJO){
                  PORTB |= (1 << PB0);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  delayVar(ledTime);
                } else if (randNum == AMARI){
                  PORTB |= (1 << PB2);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  delayVar(ledTime);
                  } else if (randNum == VERDE){
                  PORTB |= (1 << PB1);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  delayVar(ledTime);
                } else if (randNum == AZUL){
                  PORTB |= (1 << PB3);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  delayVar(ledTime);
                }
            }
      break;

      case secuence:
        _delay_ms(10000);
        for (int i = 0; i<= counterSecuence -1; i++){
        state = user; 
          bPressed = 0;

        if (secuenceLed[i]==ROJO){
            while (!bPressed) {
                if (PIND & (1 << PD0)) {
                     PORTB |= (1 << PB0);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  _delay_ms(100);
                  bPressed = 1;
                } else if ((PIND & (1 << PD1))||
                (PIND & (1 << PD2))||(PIND & (1 << PD3))){
                  state=standby;
                  goto inicio;
                }}}

         else if (secuenceLed[i]==AMARI){
            while (!bPressed) {
                if (PIND & (1 << PD2)) {
                     PORTB |= (1 << PB2);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  _delay_ms(100);
                  bPressed = 1;
                } else if ((PIND & (1 << PD0))||
                (PIND & (1 << PD1))||(PIND & (1 << PD3))){
                  state=standby;
                  goto inicio;
                }}}

         else if (secuenceLed[i]==VERDE){
            while (!bPressed) {
                if (PIND & (1 << PD1)) {
                     PORTB |= (1 << PB1);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  _delay_ms(100);
                  bPressed = 1;
                } else if ((PIND & (1 << PD0))||
                (PIND & (1 << PD2))||(PIND & (1 << PD3))){
                  state=standby;
                  goto inicio;
                }}}
         else if (secuenceLed[i]==AZUL){
            while (!bPressed) {
                if (PIND & (1 << PD3)) {
                     PORTB |= (1 << PB3);
                  delayVar(ledTime);
                  PORTB = 0x00;
                  _delay_ms(100);
                  bPressed = 1;
                } else if ((PIND & (1 << PD0))||
                (PIND & (1 << PD1))||(PIND & (1 << PD2))){
                  state=standby;
                  goto inicio;
                }}}
        }

      break;

      case user:
        state = start;
        counterSecuence++;
        ledTime = ledTime - 2000;
      break;
      default:
        state = standby;
      break;
    }
  }
}

void delayVar(uint16_t delay) {
    while (delay--) {
        _delay_ms(1); // Retarda 1 ms en cada iteración
    }
}
