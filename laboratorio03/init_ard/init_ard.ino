#include <TaskScheduler.h>
#include <math.h>

Scheduler voltmeter;

// Funciones para mapeo y obtención de valor rms
float mapInput(int inputValue, int limitValue);
float peakVoltage(float arr[], int size);

// Funciones de muestreo y display
void readVoltage();
void mapVoltage();
void showVoltage();

// Inicialización de variables
int valueA0, valueA1, valueA2, valueA3;
float mappedA0, mappedA1, mappedA2, mappedA3;
float v0, v1, v2, v3;

// Variables de control
int isAC;
int count = 0;

// Arreglos para guardar valores de AC y poder sacar la tensión pico
float valoresA0[17];
float valoresA1[17];
float valoresA2[17];
float valoresA3[17];

// Tarea para leer tensión eléctrica
Task read(1, TASK_FOREVER, &readVoltage, &voltmeter, true);
Task mapping(1, TASK_FOREVER, &mapVoltage, &voltmeter, true);
Task showing(2000, TASK_FOREVER, &showVoltage, &voltmeter, true);

void readVoltage() {
  // Leer valores de A0, A1, A2 y A3
  valueA0 = analogRead(A0);
  valueA1 = analogRead(A1);
  valueA2 = analogRead(A2);
  valueA3 = analogRead(A3);
}

void mapVoltage() {
  isAC = digitalRead(A4);

  mappedA0 = mapInput(valueA0, 34);  // El valor es 34 debido a que en alterna se medira tensión pico
  mappedA1 = mapInput(valueA1, 34);  // Por lo que se debe medir 24 * raiz(2)
  mappedA2 = mapInput(valueA2, 34);  // ya que se pide el valor rms, y para esto se necesita saber el valor pico
  mappedA3 = mapInput(valueA3, 34);  // para posteriormente dividirlo por raiz(2)

  if (isAC) {                             // Si es alterna buscaremos el valor pico
    valoresA0[count] = mappedA0;
    valoresA1[count] = mappedA1;
    valoresA2[count] = mappedA2;
    valoresA3[count] = mappedA3;
    count++;

    if (count == 17) {
      count = 0;
      v0 = peakVoltage(valoresA0, 17);
      v1 = peakVoltage(valoresA1, 17);
      v2 = peakVoltage(valoresA2, 17);
      v3 = peakVoltage(valoresA3, 17);

      for (int i = 0; i < 17; i++) {
        valoresA0[i] = 0;
        valoresA1[i] = 0;
        valoresA2[i] = 0;
        valoresA3[i] = 0;
      }
    }
  } else {            // Si es DC guardamos el valor directamente
    v0 = mappedA0;
    v1 = mappedA1;
    v2 = mappedA2;
    v3 = mappedA3;

  }
};

void showVoltage() {
  // Imprimir valores mapeados en el Monitor Serial
  Serial.print("Mapped A0: ");
  Serial.println(v0);
  Serial.print("Mapped A1: ");
  Serial.println(v1);
  Serial.print("Mapped A2: ");
  Serial.println(v2);
  Serial.print("Mapped A3: ");
  Serial.println(v3);

  Serial.println();  // Línea en blanco para separar las lecturas
};


float mapInput(int inputValue, int limitValue) {
  float m = 2 * limitValue / 1023.0;  // Pendiente de la recta
  float b = -limitValue;              // b
  return m * inputValue + b;
}

float peakVoltage(float arr[], int size) {
  float maximo = arr[0];  // Inicializa el máximo con el primer elemento del arreglo
  for (int i = 1; i < size; i++) {
    if (arr[i] > maximo) {
      maximo = arr[i];  // Actualiza el máximo si el elemento actual es mayor
    }
  }
  return maximo/sqrt(2);
}

void setup() {
  // Instrucciones iniciales
  Serial.begin(9600);
  pinMode(A4, INPUT);  // Configura A4 como entrada digital para lectura de AC/DC
}

void loop() {
  // Instrucciones en bucle
  voltmeter.execute();
}
