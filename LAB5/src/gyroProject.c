/* main.c */
/* Programa principal que inicializa el sistema y muestra los datos del giroscopio */

#include <stdio.h>
#include <stdint.h>
#include "clock.h"
#include "console.h"
#include "gyro.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"

int main(void)
{
    /* Inicializar el sistema */
    clock_setup();
    console_setup();
    gyro_setup();
    gyro_calibrate();
    sdram_init();
	lcd_spi_init();
	gfx_init(lcd_draw_pixel, 240, 320);
    /* Calibrar el giroscopio */

    int16_t x, y, z;
    float x_rate, y_rate, z_rate;
    
    char buffer_x[32];
    char buffer_y[32];
    char buffer_z[32];
    
    
    gfx_fillScreen(LCD_BLACK);
    gfx_setTextSize(3);

    /* Bucle principal */
    while (1)
    {
        /* Leer datos del giroscopio */
        gyro_read_xyz(&x, &y, &z);
        filtro_dig(&x, &x_rate);
        filtro_dig(&y, &y_rate);
        filtro_dig(&z, &z_rate);
        
		gfx_setTextColor(LCD_RED, LCD_BLACK);
        gfx_setCursor(15, 70);
        sprintf(buffer_x, "x: %.2f", x_rate);
        gfx_puts(buffer_x);
		
        gfx_setTextColor(LCD_GREEN, LCD_BLACK);
        gfx_setCursor(15, 120);
        sprintf(buffer_y, "y: %.2f", y_rate);
        gfx_puts(buffer_y);

		gfx_setTextColor(LCD_BLUE, LCD_BLACK);
        gfx_setCursor(15, 170);
        sprintf(buffer_z, "z: %.2f", z_rate);
        gfx_puts(buffer_z);
        
        printf("%.2f,%.2f,%.2f\n", x_rate, y_rate, z_rate);

        msleep(100); // Esperar 100 ms
        lcd_show_frame();
        gfx_fillScreen(LCD_BLACK);
    }

    return 0;
}

