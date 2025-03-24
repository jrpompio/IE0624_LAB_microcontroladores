/******************************************************************************
 * ________________________UNIVERSIDAD_DE_COSTA_RICA____________________________
 *__________________IE0624_Laboratorio_de_microcontroladores___________________
 *__________________________Junior_Ruiz_Sánchez________________________________
 * Laboratorio 1:
 *****************************************************************************/
#include <pic14/pic12f683.h>

#define TIEMPO 500        // Tiempo para mantener el valor obtenido en pantalla

int __at 0x2007 __config =
(_CP_OFF & _CPD_OFF      // Desactiva protecciones de codigo y de EEPROM
& _BOREN_OFF            // Desactiva reinicio por mala alimentación
& _MCLRE_ON            // Desactiva el reset para usar GP3 como input
& _WDT_OFF              // Desactiva wathdog timer
& _INTRC_OSC_NOCLKOUT); // oscilador interno, sin salida de clock


// *************** PROTOTIPOS DE FUNCIONES ********************

void delay(unsigned int tiempo);
/*  Esta función itera n*m veces donde n es el valor ingresado en la función
 *   y m corresponde a un valor por tanteo
 */
void send_face();
/*  Esta función se encarga de enviar a el registro 
 *  desplazante la cara del dado a mostrar.
 */
// CODIGO PRINCIPAL
void main(void)
{
    ANSEL = 0x00;         // Configurando pines como digitales
    CMCON0 = 0x07;        // Desactivando comparadores
    TRISIO = 0b00000100;  //  GP2 entrada, los demás son entradas
    GPIO &= (0b00000100); // Inicializa todos los pines de salida en bajo
    
    while(1){
        GP0 = 1;
        delay(TIEMPO);
        GP0 = 0;
        delay(TIEMPO);
    }
}







// *********************** FUNCIONES ***************************

void delay(unsigned int tiempo)
{
    unsigned int i;
    unsigned int j;

    for (i = 0; i < tiempo; i++)
        for (j = 0; j < 265; j++);
}

