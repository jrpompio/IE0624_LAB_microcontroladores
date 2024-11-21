# Makefile
# Archivo de construcción para compilar el proyecto del giroscopio utilizando libopencm3

# Lista de archivos objeto
OBJS = clock.o console.o gyro.o sdram.o lcd-spi.o gfx.o

# Nombre del binario
BINARY = gyroProject

# Estándar de C a utilizar
CSTD = -std=gnu99

# Si utilizas funciones matemáticas como sin/cos (puedes eliminar esta línea si no las usas)
LDLIBS += -lm

# Script de enlace y ruta a libopencm3
LDSCRIPT = makeHeaders/stm32f429i-discovery.ld
OPENCM3_DIR = /home/pompo/Universidad/LABmicrocontroladores/libopencm3

# Incluir las reglas y configuraciones comunes
include makeHeaders/Makefile.include

