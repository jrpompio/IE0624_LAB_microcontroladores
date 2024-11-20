#!/usr/bin/bash
make
arm-none-eabi-objcopy -O binary gyroProject.elf firmware.bin # el .elf se tiene que convertir a .bin con esta linea
st-flash --reset write firmware.bin 0x8000000 # esta linea es para montar el binario en el micro controlador 
rm *.d *.elf *.map *.o *.bin
