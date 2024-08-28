#include <pic14/pic12f675.h>

int __at 0x2007 __config =
 	_CP_OFF & _CPD_OFF  // Desactiva protecciones de codigo y de EEPROM
  	& _BODEN_OFF 		// Desactiva reinicio por mala alimentación
	& _MCLRE_OFF 		// Desactiva el reset para usar GP3 como input
	& _WDT_OFF 			// Desactiva wathdog timer
	& _INTRC_OSC_NOCLKOUT;  // oscilador interno, sin salida de clock 

void delay(unsigned int tiempo);

void main(void)
{
    ANSEL = 0x00;        // Configurando pines como digitales
    CMCON = 0x07;        // Desactivando comparadores
    TRISIO = 0b00000010; // GP1 como entrada, GP0 como salida
    GPIO = 0x00; // Inicializa todos los pines en bajo

    unsigned int time = 100;

    while (1)
    {


        delay(time);
		
        if (GP1)
        {
            GP0 = ~GP0;
        } 
    }
}

void delay(unsigned int tiempo)
{
    unsigned int i;
    unsigned int j;

    for (i = 0; i < tiempo; i++)
        for (j = 0; j < 1275; j++);
}
