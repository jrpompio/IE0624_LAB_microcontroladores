#!/bin/bash

# Ejecutar el script de Python en segundo plano
python3 moveCap.py &
# Obtener el PID del proceso en segundo plano
PID=$!

# Esperar 15 segundos
sleep 15

# Detener el proceso de Python
kill $PID

echo "El script moveVertical.py se ha detenido después de 15 segundos."
