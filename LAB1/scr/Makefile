FAMILY=-mpic14
MCU=-p12f683
CC=sdcc
LD=sdcc
CFLAGS=-I. -I/usr/local/share/sdcc/non-free/include
TARGET=lab1

SRCS = lab1.c

all:
	${CC} --use-non-free ${FAMILY} ${MCU} ${CFLAGS} -o ${TARGET} ${SRCS}
#	${LD} -o ${TARGET}.elf ${TARGET}.o
#	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
#	${SIZE} -C --mcu=${MCU} ${TARGET}.elf
	#
	# --------MOSTRANDO MEMORIA CONSUMIDA APROXIMADA
	tail -n 4 ${TARGET}.asm
#	simulide ${TARGET}.simu

clean:
	rm -f *.c~ *.h~ *.o *.elf *.hex *.asm *.cod *.lst
