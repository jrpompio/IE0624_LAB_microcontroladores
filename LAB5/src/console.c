/* console.c */
/* Código para configurar la consola y permitir el uso de printf */

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "console.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/* Definición del USART para la consola */
#define CONSOLE_USART USART1

void console_setup(void)
{
    /* Habilitar reloj para GPIOA (pines de USART1) y USART1 */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    /* Configurar pin GPIO para USART1 TX (PA9) */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);

    /* Configurar parámetros del USART1 */
    usart_set_baudrate(CONSOLE_USART, 115200);
    usart_set_databits(CONSOLE_USART, 8);
    usart_set_stopbits(CONSOLE_USART, USART_STOPBITS_1);
    usart_set_mode(CONSOLE_USART, USART_MODE_TX);
    usart_set_parity(CONSOLE_USART, USART_PARITY_NONE);
    usart_set_flow_control(CONSOLE_USART, USART_FLOWCONTROL_NONE);

    /* Habilitar USART1 */
    usart_enable(CONSOLE_USART);
}

/* Implementación de _write para redirigir printf al USART */
int _write(int file, char *ptr, int len)
{
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == '\n')
            {
                usart_send_blocking(CONSOLE_USART, '\r');
            }
            usart_send_blocking(CONSOLE_USART, ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}

