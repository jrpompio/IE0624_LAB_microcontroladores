import serial
import json

# Configurar el puerto serial
puerto = '/tmp/ttyS1'
baudrate = 9600  # Ajusta el baudrate según la configuración de tu dispositivo

# Configuración inicial del puerto serial
ser = serial.Serial(puerto, baudrate)

# Abre el archivo CSV en modo de escritura
with open(nombre_archivo, mode='w', newline='') as archivo_csv:
    escritor_csv = csv.writer(archivo_csv)
    
    # Escribe la cabecera del archivo CSV (opcional)
    escritor_csv.writerow(['Dato'])

    print("Escuchando en el puerto:", puerto)
    
    try:
        # Leer datos indefinidamente
        while True:
            if ser.in_waiting > 0:
                # Leer una línea del puerto serial
                linea = ser.readline().decode('utf-8').strip()
                
                # Escribir el dato en el archivo CSV
                escritor_csv.writerow([linea])
                print(linea)
                
    except KeyboardInterrupt:
        print("Interrupción por teclado, deteniendo el programa...")
    finally:
        # Cierra el puerto serial al finalizar
        ser.close()
        print("Puerto serial cerrado.")
