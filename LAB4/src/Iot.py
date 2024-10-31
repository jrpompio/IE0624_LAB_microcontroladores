import paho.mqtt.client as mqtt
import time
import serial
import json

# Configuración de la conexión serial
puerto = '/tmp/ttyS1'  # Cambia '/tmp/ttyS1' por tu puerto específico
ser = serial.Serial(puerto, 9600, timeout=1)
time.sleep(2)

# Configuración de ThingsBoard
THINGSBOARD_HOST = "iot.eie.ucr.ac.cr"
ACCESS_TOKEN = "dcnf5vs5u543r1a21fju"

# Configuración de MQTT
client = mqtt.Client()
client.username_pw_set(ACCESS_TOKEN)

# Función de conexión MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conexión a ThingsBoard exitosa")
    else:
        print("Error al conectar a ThingsBoard, código:", rc)

# Conectar el cliente
client.on_connect = on_connect
client.connect(THINGSBOARD_HOST, 1883, 60)
client.loop_start()

def enviar_telemetria(x, y, z, voltage):
    payload = json.dumps({
        "eje_X": x,
        "eje_Y": y,
        "eje_Z": z,
        "nivel_bateria": voltage
    })
    client.publish("v1/devices/me/telemetry", payload)
    print("Datos enviados:", payload)

while True:
    if ser.in_waiting > 0:
        # Inicializar variables para almacenar datos
        x = y = z = battery = None

        # Leer datos del puerto serial
        for _ in range(5):  # Lee 5 líneas (4 valores y 1 separador)
            linea = ser.readline().decode('utf-8').strip()
            try:
                # Extraer valores según etiqueta
                if linea.startswith("X:"):
                    x = float(linea.split(":")[1].strip())
                elif linea.startswith("Y:"):
                    y = float(linea.split(":")[1].strip())
                elif linea.startswith("Z:"):
                    z = float(linea.split(":")[1].strip())
                elif linea.startswith("Battery:"):
                    battery = float(linea.split(":")[1].strip())
            except ValueError:
                print("Error en el formato de datos:", linea)
        
        # Verificar que todos los datos se han leído correctamente
        if x is not None and y is not None and z is not None and battery is not None:
            enviar_telemetria(x, y, z, battery)
        else:
            print("Error: datos incompletos recibidos")
    
    time.sleep(1)
