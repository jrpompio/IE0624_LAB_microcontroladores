# wifi.py

import time
from machine import UART, Pin

# Configuración UART
# UART 0
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))


def send_command(cmd, delay=1):
    """
    Envia un comanto AT al ESP8266 y maneja la respuesta.

    Args:
        cmd(str): El comando AT a enviar.
        delay(int): Opcional. Tiempo de espera en segundos después de
                    enviar el comando. Default delay=1 segundo.
    Returns:
        bytes: La respuesta recibida del ESP8266, o None si no hay respuesta.
    """

    # Enviar el comando AT seguido de retorno de carro y salto de linea
    uart.write(cmd + '\r\n')
    time.sleep(delay)           # Esperar que el modulo procese el comando

    # Leer respuesta del modulo wifi ESP8266
    response = uart.read()

    if response:
        try:
            print(response.decode())
        except UnicodeError:
            print("Error al decodificar el mensaje:", response)
    else:
        print("Sin respuesta")
    return response


def wifi_connect(ssid, psswd):
    """
    Conecta el modulo wifi ESP8266 a la red wifi.

    Args:
        ssid(str): Nombre de la red wifi.
        psswd(str): Contraseña de la red wifi.
    """
    # MODO CLIENTE
    send_command('AT+CWMODE=1')

    # Conectar a la red
    print("Conectando al wifi...")
    send_command(f'AT+CWJAP="{ssid}","{psswd}"', delay=10)

    # Verificar la coneccción
    response = send_command('AT+CIFSR')  # Comando para obtener dirección ip
    if response:
        print("Dirección IP obtenida:", response.decode())
    else:
        print("No se pudo obtener dirección IP.")


if __name__ == "__main__":

    MYSSID = "Fala lalala"
    MYPASSWORD = "sopadefrijoles13"

    wifi_connect(MYSSID, MYPASSWORD)
    print("Conexión wifi realizada.")
