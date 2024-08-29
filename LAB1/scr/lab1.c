#include <pic14/pic12f683.h>

int __at 0x2007 __config =
    (_CP_OFF & _CPD_OFF      // Desactiva protecciones de codigo y de EEPROM
     & _BOREN_OFF            // Desactiva reinicio por mala alimentación
     & _MCLRE_OFF            // Desactiva el reset para usar GP3 como input
     & _WDT_OFF              // Desactiva wathdog timer
     & _INTRC_OSC_NOCLKOUT); // oscilador interno, sin salida de clock

void delay(unsigned int tiempo);
void display(unsigned int valor);
void parpadear();

void main(void)
{
  ANSEL = 0x00;         // Configurando pines como digitales
  CMCON0 = 0x07;        // Desactivando comparadores
  TRISIO = 0b00000100;  //
  GPIO &= (0b00000100); // Inicializa todos los pines en bajo

  unsigned int aleatorio = 99;
  unsigned int intentos = 11;
  int a[10];

  reinicio:
  parpadear();
  intentos = 10;
  for (int i = 0; i < 10; i++)
  {
    a[i] = 150; // reiniciando valores
  }

  while (1)
  {
    if (GP2)
    {
      int no_repetido = 1;
      for (int i = 0; i < 10; i++)
      {
        if (aleatorio == a[i])
        {
          no_repetido = 0;
          break;
        }
      }
      if (no_repetido == 1)
      {
        display(aleatorio);
        a[10 - intentos] = aleatorio;
        intentos--;
        delay(500);
      }
    }

    if (aleatorio == 0)
    {
      aleatorio = 100;
    }
    if (intentos == 0)
    {
      goto reinicio;
    }

    aleatorio--;
    display(aleatorio);
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
  unsigned short int semiCiclo = 16; // Se necesitan enviar 8 bits
  unsigned short int bit_enviar = 0; // Se necesitan enviar 8 bits
  GP0 = 0;
  int decenas = valor / 10;
  int unidades = valor % 10;
  decenas = (decenas << 4);
  valor = decenas | unidades;
  while (semiCiclo > 0)
  {
    if (GP0 == 0)
    {
      GP1 = ((valor >> bit_enviar) // Se obtiene el digito más significativo
             & 0x01);              // se shiftea 7 veces
      bit_enviar++;
      delay(1); // delay de seguridad para que
                // no se guarde el digito antes de cambiarlo
    }
    GP0 = ~GP0;
    semiCiclo--;
    delay(1);
  }
}

void parpadear()
{
  display(99);
  delay(100);
  display(150);
  delay(50);
  display(99);
  delay(100);
  display(150);
  delay(50);
  display(99);
  delay(100);
  display(150);
  delay(50);
}