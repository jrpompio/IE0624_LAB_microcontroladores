/* clock.h */
/* Archivo de cabecera para la configuración del reloj y temporización */

#ifndef __CLOCK_H
#define __CLOCK_H

#include <stdint.h>

/* Función para configurar el reloj del sistema y SysTick */
void clock_setup(void);

/* Función para esperar un número de milisegundos */
void msleep(uint32_t delay);

/* Función para obtener el tiempo en milisegundos desde el arranque */
uint32_t mtime(void);

/* Handler para SysTick */
// void sys_tick_handler(void);

#endif /* __CLOCK_H */

