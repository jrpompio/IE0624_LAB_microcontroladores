/* gyro.h */
/* Archivo de cabecera para la configuración y lectura del giroscopio */

#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>

/* Función para configurar el giroscopio */
void gyro_setup(void);


/* Función para calibrar el giroscopio */
void gyro_calibrate(void);

/* Función para leer los ejes X, Y, Z del giroscopio */
void gyro_read_xyz(int16_t *x, int16_t *y, int16_t *z);

/* Función para aplicar un filtro de datos */
void filtro_dig(int16_t *w, float *w_rate);

#endif /* GYRO_H */

