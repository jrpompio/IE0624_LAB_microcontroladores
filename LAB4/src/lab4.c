#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define USART_CONSOLE USART1
#define SPI_BUS SPI5

static void clock_setup(void);
static void usart_setup(void);
int _write(int file, char *ptr, int len);

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOF);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_SPI5);
}

static void usart_setup(void) {
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);

    usart_set_baudrate(USART_CONSOLE, 115200);
    usart_set_databits(USART_CONSOLE, 8);
    usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
    usart_set_mode(USART_CONSOLE, USART_MODE_TX);
    usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
    usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

    usart_enable(USART_CONSOLE);
}

int _write(int file, char *ptr, int len) {
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        for (i = 0; i < len; i++) {
            if (ptr[i] == '\n') {
                usart_send_blocking(USART_CONSOLE, '\r');
            }
            usart_send_blocking(USART_CONSOLE, ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}

// Función para configurar el SysTick
static void systick_setup(void) {
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(168000); // 1ms con reloj de 168MHz
    systick_interrupt_enable();
    systick_counter_enable();
}

volatile uint32_t msTicks;

void sys_tick_handler(void) {
    msTicks++;
}

static void delay_ms(uint32_t delay) {
    uint32_t current_tick = msTicks;
    while ((msTicks - current_tick) < delay);
}




int main(void) {
    clock_setup();
    usart_setup();
    systick_setup();
    while (1)
    {
        printf("hola mundo :3\n");
        delay_ms(1000);
    }
    
    
    return 0;
}