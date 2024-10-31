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
#define GYRO_CS_PORT GPIOC
#define GYRO_CS_PIN GPIO1

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

static void spi_setup(void) {
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7 | GPIO8 | GPIO9);
    gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

    gpio_mode_setup(GYRO_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GYRO_CS_PIN);
    gpio_set(GYRO_CS_PORT, GYRO_CS_PIN);

    spi_disable(SPI_BUS);
    spi_set_master_mode(SPI_BUS);
    spi_set_baudrate_prescaler(SPI_BUS, SPI_CR1_BR_FPCLK_DIV_64);
    spi_set_clock_polarity_0(SPI_BUS);
    spi_set_clock_phase_1(SPI_BUS);
    spi_set_full_duplex_mode(SPI_BUS);
    spi_enable_software_slave_management(SPI_BUS);
    spi_send_msb_first(SPI_BUS);
    spi_set_nss_high(SPI_BUS);
    spi_enable(SPI_BUS);
}

static void gyro_setup(void) {
    // Habilitar el giroscopio, configurar el rango de medición a 250 dps,
    // deshabilitar el filtro de paso alto y habilitar los 3 ejes
    gpio_clear(GYRO_CS_PORT, GYRO_CS_PIN);
    spi_xfer(SPI_BUS, 0x20); // CTRL_REG1
    spi_xfer(SPI_BUS, 0xCF); // Activar ejes X, Y, Z y 760 Hz (DR1=1, DR0=1)
//    spi_xfer(SPI_BUS, 0x23); // CTRL_REG4
//    spi_xfer(SPI_BUS, 0x00); // 250 dps (FS1 = 0, FS0 = 0) 
//    spi_xfer(SPI_BUS, 0x24); // CTRL_REG5
//    spi_xfer(SPI_BUS, 0x00); // Deshabilitar filtro paso alto (HPen = 0)
    gpio_set(GYRO_CS_PORT, GYRO_CS_PIN);
}

static uint8_t gyro_read_who_am_i(void) {
    uint8_t who_am_i;

    gpio_clear(GYRO_CS_PORT, GYRO_CS_PIN);
    spi_xfer(SPI_BUS, 0x0F | 0x80); // Lectura del registro WHO_AM_I
    who_am_i = spi_xfer(SPI_BUS, 0x00);
    gpio_set(GYRO_CS_PORT, GYRO_CS_PIN);

    return who_am_i;
}



int main(void) {
    clock_setup();
    usart_setup();
    systick_setup();
    spi_setup();
    gyro_setup();

    uint8_t who_am_i = gyro_read_who_am_i();

    while (1)
    {
        printf("WHO_AM_I: 0x%X\n", who_am_i);
        delay_ms(1000);
    }
    
    
    return 0;
}