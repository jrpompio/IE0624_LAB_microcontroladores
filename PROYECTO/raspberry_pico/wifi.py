# wifi.py
import time
from machine import UART, Pin

# Configuración UART
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

def await_response(timeout=5):
    start_time = time.time()
    while time.time() - start_time < timeout:
        response = uart.read()
        if response:
            return response
    return None

def send_command(cmd, timeout=5):
    uart.write(cmd + '\r\n')
    response = await_response(timeout)
    if response:
        try:
            print(response.decode())
        except UnicodeError:
            print("Error al decodificar el mensaje. Respuesta cruda:", response)
    else:
        print("Sin respuesta")
    return response

def wifi_connect(ssid, password):
    print("Reiniciando el módulo...")
    send_command('AT+RST', timeout=2)
    time.sleep(3)  # Pausa adicional tras el reinicio

    print("Configurando modo cliente...")
    send_command('AT+CWMODE=1', timeout=2)

    print("Conectando al WiFi...")
    response = send_command(f'AT+CWJAP="{ssid}","{password}"', timeout=15)
    time.sleep(5)  # Pausa adicional tras el comando AT+CWJAP

    if response:
        if b"WIFI CONNECTED" in response or b"WIFI GOT IP" in response:
            print("Conexión exitosa.")
        elif b"WIFI DISCONNECT" in response:
            print("El módulo se desconectó inicialmente, intentando reconectar...")
        else:
            print("Error al conectar a WiFi. Respuesta completa:")
            print(response.decode())
    else:
        print("No hubo respuesta al intentar conectar al WiFi.")

    print("Obteniendo información de red...")
    response = send_command('AT+CIFSR', timeout=5)
    if response:
        print("Datos de red recibidos:\n", response.decode())
    else:
        print("No se pudo obtener la información de red.")

