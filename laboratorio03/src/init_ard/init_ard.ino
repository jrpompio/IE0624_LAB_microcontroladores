#include <TaskScheduler.h>
#include <math.h>
#include <Adafruit_GFX.h>      // Librería gráfica
#include <Adafruit_PCD8544.h>  // Librería para la pantalla Nokia 5110

// Definir pines de conexión
#define RST_PIN   8
#define CE_PIN    7
#define DC_PIN    6
#define DIN_PIN   5
#define CLK_PIN   4

// Inicializar la pantalla
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);

// Objeto para multitasking
Scheduler voltmeter;

// Funciones de pantalla
void screen(float voltage0, float voltage1, float voltage2, float voltage3);
void serialOut(float voltage0, float voltage1, float voltage2, float voltage3);

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
Task showing(500, TASK_FOREVER, &showVoltage, &voltmeter, true);

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
  screen(v0, v1, v2, v3);
  serialOut(v0, v1, v2, v3);
  if (abs(v0) > 20 || abs(v1) > 20 || abs(v2) > 20 || abs(v3) > 20) {
    digitalWrite(13, HIGH);  // Enciende el led de seguridad
  } else {
    digitalWrite(13, LOW);   // Apaga el led
  }
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

void screen(float voltage0, float voltage1, float voltage2, float voltage3){
  // Limpiar pantalla
  display.clearDisplay();

  // Mostrar el valor del sensor
  display.setCursor(0, 0);
  display.print("in0: ");
  display.print(voltage0);
  display.println("V");

  display.setCursor(0, 10);
  display.print("in1: ");
  display.print(voltage1);
  display.println("V");

  display.setCursor(0, 20);
  display.print("in2: ");
  display.print(voltage2);
  display.println("V");

  display.setCursor(0, 30);
  display.print("in3: ");
  display.print(voltage3);
  display.println("V");

  display.setCursor(0, 41);
  display.print("Vmax: 24V");

  // Mostrar en pantalla
  display.display();

  }

void serialOut(float voltage0, float voltage1, float voltage2, float voltage3){
  Serial.print("Input 0: ");
  Serial.print(voltage0);
  Serial.println(" V");

  Serial.print("Input 1: ");
  Serial.print(voltage1);
  Serial.println(" V");

  Serial.print("Input 2: ");
  Serial.print(voltage2);
  Serial.println(" V");

  Serial.print("Input 3: ");
  Serial.print(voltage3);
  Serial.println(" V");

  Serial.println("-------------------");  // Separador entre lecturas

}

void setup() {
  // Instrucciones iniciales
  Serial.begin(9600);
  pinMode(A4, INPUT);  // Configura A4 como entrada digital para lectura de AC/DC
  pinMode(13, OUTPUT);
  // Inicializar la pantalla
  display.begin();
  display.setContrast(50);   // Ajustar el contraste de la pantalla
  // Limpiar pantalla
  display.clearDisplay();
  display.setTextSize(0.1);
    // Escribir texto
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Hola!");
  display.println("PCD8544");

  // Mostrar cambios en pantalla
  display.display();
}

void loop() {
  // Instrucciones en bucle
  voltmeter.execute();
}
