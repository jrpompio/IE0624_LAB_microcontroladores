import time
import serial
import csv

# Configuración de la conexión serial
puerto = '/dev/ttyACM0'  # Cambia esto por el puerto específico de tu STM32
ser = serial.Serial(puerto, 115200, timeout=1)

# Nombre del archivo CSV
nombre_archivo_csv = "estacionario.csv"

# Inicializa el archivo CSV
with open(nombre_archivo_csv, mode='w', newline='') as archivo_csv:
    writer = csv.writer(archivo_csv)
    print(f"Archivo {nombre_archivo_csv} creado. Listo para registrar datos crudos.")

while True:
    if ser.in_waiting > 0:
        # Leer una línea completa del puerto serial
        linea = ser.readline().decode('utf-8').strip()
        try:
            # Verificar que la línea contiene tres valores separados por comas
            valores = linea.split(",")
            if len(valores) == 3:
                # Convertir a flotantes y guardar en el archivo CSV
                datos = [float(val.strip()) for val in valores]
                with open(nombre_archivo_csv, mode='a', newline='') as archivo_csv:
                    writer = csv.writer(archivo_csv)
                    writer.writerow(datos)
                print(f"Datos guardados: {datos}")
            else:
                print(f"Formato incorrecto: {linea}")
        except ValueError:
            print(f"Error al procesar la línea: {linea}")
    
    time.sleep(0.01)  # Esperar un corto periodo antes de leer nuevamente
