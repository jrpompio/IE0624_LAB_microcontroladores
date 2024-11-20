/* clock.c */
/* Código para configurar el reloj del sistema y temporización */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include "clock.h"

/* Variable para contar los milisegundos desde el arranque */
static volatile uint32_t system_millis;

/* Handler para SysTick */
void sys_tick_handler(void)
{
    system_millis++;
}

/* Función para esperar un número de milisegundos */
void msleep(uint32_t delay)
{
    uint32_t wake = system_millis + delay;
    while (wake > system_millis);
}

/* Función para obtener el tiempo en milisegundos desde el arranque */
uint32_t mtime(void)
{
    return system_millis;
}

/* Configuración del reloj del sistema y SysTick */
void clock_setup(void)
{
    /* Configurar el reloj del sistema a 168 MHz */
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Configurar SysTick para generar interrupciones cada 1 ms */
    systick_set_reload(168000);  // 168 MHz / 168000 = 1000 Hz (1 ms)
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}

