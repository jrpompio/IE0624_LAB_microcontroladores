/* gyro.c */
/* Código para configurar y leer datos del giroscopio mediante SPI */

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "gyro.h"
#include "clock.h" // Para usar msleep

/* Definiciones de pines y periféricos para el giroscopio */
#define GYRO_SPI        SPI5
#define GYRO_SPI_RCC    RCC_SPI5
#define GYRO_GPIO_PORT  GPIOF
#define GYRO_GPIO_RCC   RCC_GPIOF
#define GYRO_CS_PORT    GPIOC
#define GYRO_CS_RCC     RCC_GPIOC
#define GYRO_SCK        GPIO7   /* PF7 */
#define GYRO_MISO       GPIO8   /* PF8 */
#define GYRO_MOSI       GPIO9   /* PF9 */
#define GYRO_CS         GPIO1   /* PC1 */

/* Direcciones de registros del giroscopio */
#define WHO_AM_I        0x0F
#define CTRL_REG1       0x20
#define CTRL_REG2       0x21
#define CTRL_REG4       0x23
#define OUT_X_L         0x28
#define READ_FLAG       0x80

/* Offsets para calibración */
static int16_t offset[3] = {0, 0, 0}; // Offsets para X, Y, Z

/* Prototipos de funciones internas */
static void gyro_write_register(uint8_t reg, uint8_t value);
static uint8_t gyro_read_register(uint8_t reg);

/* Función para configurar el giroscopio */
void gyro_setup(void)
{
    /* Habilitar relojes para SPI y GPIO */
    rcc_periph_clock_enable(GYRO_SPI_RCC);
    rcc_periph_clock_enable(GYRO_GPIO_RCC);
    rcc_periph_clock_enable(GYRO_CS_RCC);

    /* Configurar pines GPIO para SPI5: SCK, MISO, MOSI */
    gpio_mode_setup(GYRO_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE,
                    GYRO_SCK | GYRO_MISO | GYRO_MOSI);
    gpio_set_af(GYRO_GPIO_PORT, GPIO_AF5, GYRO_SCK | GYRO_MISO | GYRO_MOSI);

    /* Configurar pin GPIO para CS */
    gpio_mode_setup(GYRO_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GYRO_CS);
    gpio_set(GYRO_CS_PORT, GYRO_CS); // CS inactivo (alto)

    /* Configurar parámetros del SPI */
    spi_set_unidirectional_mode(GYRO_SPI);         // Modo unidireccional de 2 líneas
    spi_disable_crc(GYRO_SPI);                     // Deshabilitar cálculo CRC
    spi_set_dff_8bit(GYRO_SPI);                    // Tamaño de datos de 8 bits
    spi_set_full_duplex_mode(GYRO_SPI);            // Comunicación full duplex
    spi_enable_software_slave_management(GYRO_SPI);
    spi_set_nss_high(GYRO_SPI);                    // NSS en alto
    spi_set_baudrate_prescaler(GYRO_SPI, SPI_CR1_BR_FPCLK_DIV_64); // Velocidad del SPI
    spi_set_master_mode(GYRO_SPI);                 // Modo maestro
    spi_set_clock_polarity_0(GYRO_SPI);            // CPOL = 0
    spi_set_clock_phase_1(GYRO_SPI);               // CPHA = 1
    spi_enable(GYRO_SPI);                          // Habilitar SPI

    /* Verificar la identidad del giroscopio */
    uint8_t who_am_i = gyro_read_register(WHO_AM_I);
    if (who_am_i != 0xD4)
    {
        // Giroscopio no detectado
        while (1);
    }

    /* Configurar registros del giroscopio */
    gyro_write_register(CTRL_REG1, 0xFF); // Habilitar ejes, modo normal, data rate de 95 Hz
//    gyro_write_register(CTRL_REG2, 0xA1);
    gyro_write_register(CTRL_REG4, 0x00); // Actualización continua, escala de 250 dps
}

/* Función para calibrar el giroscopio */
void gyro_calibrate(void)
{
    int32_t sum[3] = {0, 0, 0};
    int16_t x, y, z;

    for (int i = 0; i < 100; i++)
    {
        gyro_read_xyz(&x, &y, &z);
        sum[0] += x;
        sum[1] += y;
        sum[2] += z;
        msleep(10);
    }

    offset[0] = sum[0] / 100;
    offset[1] = sum[1] / 100;
    offset[2] = sum[2] / 100;
}

/* Función para leer los ejes X, Y, Z del giroscopio */
void gyro_read_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6];

    gpio_clear(GYRO_CS_PORT, GYRO_CS); // CS bajo

    /* Leer múltiples bytes desde OUT_X_L con auto-incremento */
    spi_send(GYRO_SPI, READ_FLAG | 0x40 | OUT_X_L);
    spi_read(GYRO_SPI); // Lectura dummy

    for (int i = 0; i < 6; i++)
    {
        spi_send(GYRO_SPI, 0x00); // Escritura dummy
        data[i] = spi_read(GYRO_SPI);
    }

    gpio_set(GYRO_CS_PORT, GYRO_CS); // CS alto

    /* Combinar bytes altos y bajos, y restar offset */
    *x = (int16_t)(data[1] << 8 | data[0]) - offset[0];
    *y = (int16_t)(data[3] << 8 | data[2]) - offset[1];
    *z = (int16_t)(data[5] << 8 | data[4]) - offset[2];
}

/* Función interna para leer un registro del giroscopio */
static uint8_t gyro_read_register(uint8_t reg)
{
    uint8_t value;

    gpio_clear(GYRO_CS_PORT, GYRO_CS); // CS bajo

    spi_send(GYRO_SPI, READ_FLAG | reg);
    spi_read(GYRO_SPI); // Lectura dummy

    spi_send(GYRO_SPI, 0x00); // Escritura dummy
    value = spi_read(GYRO_SPI);

    gpio_set(GYRO_CS_PORT, GYRO_CS); // CS alto

    return value;
}

/* Función interna para escribir en un registro del giroscopio */
static void gyro_write_register(uint8_t reg, uint8_t value)
{
    gpio_clear(GYRO_CS_PORT, GYRO_CS); // CS bajo

    spi_send(GYRO_SPI, reg);
    spi_read(GYRO_SPI); // Lectura dummy

    spi_send(GYRO_SPI, value);
    spi_read(GYRO_SPI); // Lectura dummy

    gpio_set(GYRO_CS_PORT, GYRO_CS); // CS alto
}

