/******************************************************************************
 * ________________________UNIVERSIDAD_DE_COSTA_RICA____________________________
 *__________________IE0624_Laboratorio_de_microcontroladores___________________
 *__________________________Junior_Ruiz_Sánchez________________________________
 * Laboratorio 1:
 *  En este laboratorio se realizará un dado electronico que simula uno real,
 *  Se usará el microcontrolador PIC 12F683 para este proposito.
 *****************************************************************************/
#include <pic14/pic12f683.h>

#define TIEMPO 100 // Tiempo para mantener el valor obtenido en pantalla

int __at 0x2007 __config =
    (_CP_OFF & _CPD_OFF      // Desactiva protecciones de codigo y de EEPROM
     & _BOREN_OFF            // Desactiva reinicio por mala alimentación
     & _MCLRE_OFF            // Desactiva el reset para usar GP3 como input
     & _WDT_OFF              // Desactiva wathdog timer
     & _INTRC_OSC_NOCLKOUT); // oscilador interno, sin salida de clock

// *************** PROTOTIPOS DE FUNCIONES ********************

void delay(unsigned int tiempo);
/*  Esta función itera n*m veces donde n es el valor ingresado en la función
 *   y m corresponde a un valor por tanteo
 */
void send_binary(unsigned int valor);
/*
 * Se envia numero en binario al registro desplazante
 */
void send_face(unsigned int valor);
/*  Esta función se encarga de enviar a el registro
 *  desplazante la cara del dado a mostrar.
 */
// CODIGO PRINCIPAL
void main(void) {
  ANSEL = 0x00;         // Configurando pines como digitales
  CMCON0 = 0x07;        // Desactivando comparadores
  TRISIO = 0b00000100;  //  GP2 entrada, los demás son entradas
  GPIO &= (0b00000100); // Inicializa todos los pines de salida en bajo

  unsigned int face = 1;
  unsigned int animation = 1;
  unsigned int state = 1;
  unsigned int init_state1 = 1;

  while (1) {
    switch (state) {
    case 1:
      if (face % 50 == 0) {
        send_binary(animation);
        if (animation > 64)
          animation = 1;
        animation = animation * 2;
      }
      if (init_state1) {
        delay(500);
        init_state1 = 0;
      }
      if (face > 600)
        face = 1;
      if (GP2 == 1)
        state = 2;
      delay(2);
      face++;
      break;
    case 2:
      delay(500);
      send_face(face % 6 + 1);
      state = 3;
      init_state1 = 1;
      break;
    case 3:
      if (GP2 == 1)
        state = 1;
      break;
    default:
      state = 1;
      break;
    }
  }
}

// *********************** FUNCIONES ***************************

void delay(unsigned int tiempo) {
  unsigned int i;
  unsigned int j;

  for (i = 0; i < tiempo; i++)
    for (j = 0; j < 265; j++)
      ;
}

void send_binary(unsigned int valor) {
  GP0 = 0; // Iniciar tren de pulsos en bajo

  for (unsigned int i = 0; i < 8; i++) {
    // Poner el bit i (LSB primero) en GP1
    GP1 = (valor >> i) & 0x01;
    delay(1);

    // Subir el clock para que el registro muestree
    GP0 = 1;
    delay(1);

    // Bajar el clock
    GP0 = 0;
    delay(1);
  }
}

void send_face(unsigned int valor) {
  switch (valor) {
  case 1:
    // Leds: [7]
    // binario: 0100_0000
    // dec: 64
    valor = 64;
    break;
  case 2:
    // Leds: [1,4]
    // binario: 0000_1001
    // dec: 9
    valor = 9;
    break;
  case 3:
    // Leds: [1,4,7]
    // binario: 0100_1001
    // dec: 73
    valor = 73;
    break;
  case 4:
    // Leds: [1,3,4,6]
    // binario: 0010_1101
    // dec: 45
    valor = 45;
    break;
  case 5:
    // Leds: [1,3,4,6,7]
    // binario: 0110_1101
    // dec: 109
    valor = 109;
    break;
  case 6:
    // Leds: [1,2,3,4,5,6]
    // binario: 0011_1111
    // dec: 63
    valor = 63;
    break;
  default:
    // Apagaremos todos los leds
    // dec: 0
    valor = 0;
    break;
  }
  send_binary(valor);
}
