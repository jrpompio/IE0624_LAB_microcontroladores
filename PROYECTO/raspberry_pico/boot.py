# boot.py
import machine
import time

def parpadear_led_integrado(veces, intervalo):
    """
    Hace que el LED integrado parpadee un número específico de veces.

    Args:
        veces (int): Número de veces que el LED debe parpadear.
        intervalo (float): Tiempo en segundos que el LED estará encendido y apagado.
    """
    # Configurar el pin del LED integrado
    # En muchos dispositivos ESP8266, el LED integrado está conectado al GPIO2
    LED_PIN = 25
    led = machine.Pin(LED_PIN, machine.Pin.OUT)
    
    # Asegurarse de que el LED comience apagado
    led.value(0)
    
    for _ in range(veces):
        # Encender el LED
        led.value(1)
        time.sleep(intervalo)
        
        # Apagar el LED
        led.value(0)
        time.sleep(intervalo)
    

# Parámetros de parpadeo
NUMERO_DE_PARPADEOS = 2
INTERVALO_SEGUNDOS = 0.5

# Ejecutar la función de parpadeo al iniciar
parpadear_led_integrado(NUMERO_DE_PARPADEOS, INTERVALO_SEGUNDOS)

