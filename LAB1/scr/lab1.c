#include <pic14/pic12f675.h>

#define time 1

int __at 0x2007 __config =
 	_CP_OFF & _CPD_OFF            // Desactiva protecciones de codigo y de EEPROM
  & _BODEN_OFF 		              // Desactiva reinicio por mala alimentación
	& _MCLRE_OFF 		              // Desactiva el reset para usar GP3 como input
	& _WDT_OFF 			              // Desactiva wathdog timer
	& _INTRC_OSC_NOCLKOUT;        // oscilador interno, sin salida de clock 

void delay(unsigned int tiempo);
void display(unsigned int valor);

void main(void)
{
    ANSEL = 0x00;         // Configurando pines como digitales
    CMCON = 0x07;         // Desactivando comparadores
    TRISIO = 0b00000000;  // 
    GPIO &= (0b00000000); // Inicializa todos los pines en bajo
    unsigned int aleatorio = 99;
    while (1)
    { 
      aleatorio = aleatorio - 1;
      display(aleatorio);

    }
}

void delay(unsigned int tiempo)
{
    unsigned int i;
    unsigned int j;

    for (i = 0; i < tiempo; i++)
        for (j = 0; j < 1275; j++);
}

void display(unsigned int valor){
    unsigned short int semiCiclo = 16;     // Se necesitan enviar 8 bits
    GP0 = 0;
    int decenas = valor / 10; 
    int unidades = valor % 10;
    decenas = (decenas << 4);
    valor = decenas | unidades;
    while (semiCiclo > 0)
    // DEBO LIMPIAR ESTA COCHINADA
    
    { 
      if (GP0 == 0){
        GP1 = ((valor << 
        ((16-semiCiclo)/2)
        )  // Se obtiene el digito más significativo
         & 0x80) >> 7;                // se shiftea 7 veces 
//        bit_enviar = bit_enviar + 1;
        delay(1);                     // delay de seguridad para que
                                      // no se guarde el digito antes de cambiarlo
      }
      GP0 = ~GP0;
      semiCiclo = semiCiclo - 1;
      delay(1);
    }
}
