/******************************************************************************
 * ________________________UNIVERSIDAD_DE_COSTA_RICA____________________________
 *__________________IE0624_Laboratorio_de_microcontroladores___________________
 *__________________________Junior_Ruiz_Sánchez________________________________
 * Laboratorio 1:
 *****************************************************************************/
#include <pic14/pic12f683.h>

#define TIEMPO 1000        // Tiempo para mantener el valor obtenido en pantalla

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
void send_face(unsigned int valor);
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
    unsigned int face = 1;
    while(1){
        if (face > 6) face = 1;
        send_face(face);
        delay(TIEMPO);
        send_face(face);
        delay(TIEMPO);
        face++;
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


void send_face(unsigned int valor)
{
    
    GP0 = 0;  // Iniciar tren de pulsos en bajo

    for (unsigned int i = 0; i < 8; i++)
    {
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


