# main.py

from machine import Pin
import wifi
import config  # Importa las configuraciones del archivo config.py

# Configuración WiFi y UART
ssid = config.SSID
password = config.PASSWORD
mac_address = config.MAC_ADDRESS
uart_num = 0  # UART 0
tx_pin = 0
rx_pin = 1
baudrate = 115200

not_found_message = (
            "Endpoints disponibles:\n"
            "/wol: Para encener la computadora\n"
            "/luzon: para encender el led en GPIO 25\n"
            "/luzoff: para apagar el led en GPIO 25\n"
        )

# Instancia de WiFiManager con depuración activada y mensaje personalizado
wifi_manager = wifi.WiFiManager(ssid, password, uart_num, tx_pin, rx_pin, baudrate,
                                I_WANT_TO_DEPURATE_THIS_FS=False,
                                default_not_found_message=not_found_message)

# Configuración GPIO
light = Pin(25, Pin.OUT)

# Funciones específicas para cada endpoint


def handle_wol_request():
    print("[handle_wol_request] Enviando Magic Packet...")
    wifi_manager.connect()
    wifi_manager.send_magic_packet(mac_address)
    wifi_manager.start_server()


def handle_light_on():
    print("[handle_light_on] Encendiendo luz...")
    light.value(1)


def handle_light_off():
    print("[handle_light_off] Apagando luz...")
    light.value(0)


# Configuración de la red y el servidor HTTP
wifi_manager.connect()
wifi_manager.start_server()

# Registro de endpoints
wifi_manager.add_endpoint("/wol", handle_wol_request,
                          "Encendiendo computadora.\n")
wifi_manager.add_endpoint("/luzon", handle_light_on, "Luz encendida.\n")
wifi_manager.add_endpoint("/luzoff", handle_light_off, "Luz apagada.\n")

# Escucha de solicitudes
wifi_manager.listen()
