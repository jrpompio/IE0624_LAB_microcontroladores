/* main.c */
/* Programa principal que inicializa el sistema y muestra los datos del giroscopio */

#include "clock.h"
#include "console.h"
#include "gyro.h"
#include <stdio.h>

void filtro_dig(float *w_valor);
void filtro_dig(float *w_valor) {
    if (*w_valor >= -1.0 && *w_valor <= 1.0) {
        *w_valor = 0.0; // Si está en el rango [-1.0, 1.0], lo convierte a 0.0
    }
}

int main(void)
{
    int16_t x, y, z;
    float sensitivity = 1e-3; // Sensibilidad para ±250 dps
    float x_rate, y_rate, z_rate;

    /* Inicializar el sistema */
    clock_setup();
    console_setup();
    gyro_setup();

    /* Calibrar el giroscopio */
    gyro_calibrate();

    /* Bucle principal */
    while (1)
    {
        /* Leer datos del giroscopio */
        gyro_read_xyz(&x, &y, &z);

        /* Convertir a velocidades angulares */
        x_rate = x * sensitivity;
        y_rate = y * sensitivity;
        z_rate = z * sensitivity;
        filtro_dig(&x_rate);
        filtro_dig(&y_rate);
        filtro_dig(&z_rate);

        
        /* Mostrar los valores */
        printf("%.2f,%.2f,%.2f\n", x_rate, y_rate, z_rate);

        msleep(100); // Esperar 100 ms
    }

    return 0;
}

