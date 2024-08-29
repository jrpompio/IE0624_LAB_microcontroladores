/******************************************************************************
 *________________________UNIVERSIDAD_DE_COSTA_RICA____________________________
 *__________________IE0624_Laboratorio_de_microcontroladores___________________
 *__________________________Junior_Ruiz_Sánchez________________________________
 * Laboratorio 1:
 *     Display que muestra valor aleatorio entre [00, 99]
 *     tiene 10 intentos, no se repiten números y al terminar los intentos
 *     los valores almacenados se reinician, mostrando el valor 99 _3 veces.
 *****************************************************************************/
#include <pic14/pic12f683.h>

#define TIEMPO 500

int __at 0x2007 __config =
    (_CP_OFF & _CPD_OFF      // Desactiva protecciones de codigo y de EEPROM
     & _BOREN_OFF            // Desactiva reinicio por mala alimentación
     & _MCLRE_OFF            // Desactiva el reset para usar GP3 como input
     & _WDT_OFF              // Desactiva wathdog timer
     & _INTRC_OSC_NOCLKOUT); // oscilador interno, sin salida de clock

void delay(unsigned int tiempo);
/*  Esta función itera n*m veces donde n es el valor ingresado en la función
    y m corresponde a un valor cercano a 1000. En este caso 1275. 
*/
void display(unsigned int valor);
/*  Esta función toma un valor númerico entre [00, 99]
    y lo muestra usando dos display de 7 segmentos en BCD
    pero dichos displays de 7 segmentos están conectados
    a un registro desplazante de 8 bits.
    Para ello el pin GP0 es pin que envia el tren de pulsos
    y el pin GP1 es el pin que envia los datos */
void parpadear(void);

void main(void)
{
  ANSEL = 0x00;         // Configurando pines como digitales
  CMCON0 = 0x07;        // Desactivando comparadores
  TRISIO = 0b00000100;  //  GP2 entrada, los demás son entradas
  GPIO &= (0b00000100); // Inicializa todos los pines de salida en bajo

  unsigned int aleatorio = 99;    // Valor inicial para números aleatorios
  unsigned int intentos = 10;     // cantidad de intentos
  int array[10];                  // Arreglo para guardar números y no repetir

  reinicio:                       // Etiqueta para usar goto
  parpadear();
  for (int i = 0; i < 10; i++)    // Ciclo for para formatear valores del array
  {
    array[i] = 150; // asignando un valor que no está en [00, 99]
  }

  while (1)     // MAIN LOOP
  {
    if (GP2)    // EJECUTAR SI SE PRESIONA EL BOTÓN
    {
      int no_repetido = 1;             /* Este valor sirve para verificar que
                                          el número no sea repetido          */ 
      for (int i = 0; i < 10; i++)     /* Ciclo for para revisar valores
                                          almacenados en array               */
      {
        if (aleatorio == array[i])    /* Se verifica si el valor está contenido
                                          en array*/
        {
          no_repetido = 0;             // En caso de estar contenido, el valor
                                       // de control de número no repetido
          break;                       // cambia a cero y se rompe el ciclo
        }
      }

      if (no_repetido == 1)                 // si el valor no es repetido
      {
        display(aleatorio);                 // se muestra el valor
        array[10 - intentos] = aleatorio;   // se almacena
        intentos--;                         // se resta 1 a los intentos   
        delay(TIEMPO);                      // se mantiene el valor en pantalla
      }
    }

    if (aleatorio == 0)            // control de valor del número aleatorio
    {                              // aleatorio para estar en bucle [00, 99]
      aleatorio = 100;             // valor 100 por qué se restará un 1
    }

    if (intentos == 0)            // Reiniciar, al terminar los intentos
    {
      goto reinicio;
    }

    aleatorio--;
    display(aleatorio);           // display para mostrar que está iterando
  }
}

void delay(unsigned int tiempo)
{
  unsigned int i;
  unsigned int j;

  for (i = 0; i < tiempo; i++)
    for (j = 0; j < 1275; j++)
      ;
}

void display(unsigned int valor)
{
  unsigned short int semiCiclo = 16;   // Se necesitan enviar 8 bits
  unsigned short int bit_enviar = 0;   // control de bit a enviar
  GP0 = 0;                             // se inicia el tren de pulsos en cero
  int decenas = valor / 10;            // obteniendo valor decena para BCD
  int unidades = valor % 10;           // obteniendo valor unidad para BCD
  decenas = (decenas << 4);            // Desplanzando para obtener 4 bits en 0
  valor = decenas | unidades;          // obteniendo valor en BCD
  
  while (semiCiclo > 0)                  // loop de control para tren de pulsos     
  {
    if (GP0 == 0)                        // Si el nivel del tren es low
    {
      GP1 = ((valor >> bit_enviar)  // Se envia el digito más significativo
             & 0x01);               // con la ayuda de una mascara AND
      bit_enviar++;                 // siguiente bit a enviar
      delay(1);                     // delay de seguridad para que
                                    // no se envie el digito antes de cambiarlo
    }
    GP0 = ~GP0;                     // se cambia el nivel del tren de pulsos
    semiCiclo--;                    // se resta 1 a la variable de control
    delay(1);                       // delay para mantener valor 
  }
}

void parpadear(void)
{
  display(99);
  delay(100);
  display(150);         // Se usa este valor para mostrar una F
  delay(50);            // gracias a esto se sabe que se está reiniciando
  display(99);
  delay(100);
  display(150);
  delay(50);
  display(99);
  delay(100);
  display(150);
  delay(50);
}