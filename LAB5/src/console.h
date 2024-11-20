/* console.h */
/* Archivo de cabecera para la configuración de la consola y uso de printf */

#ifndef __CONSOLE_H
#define __CONSOLE_H

/* Función para configurar la consola USART */
void console_setup(void);
int _write(int file, char *ptr, int len);

#endif /* __CONSOLE_H */

