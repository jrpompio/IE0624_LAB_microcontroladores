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

void setup() {
  // Inicializar la pantalla
  display.begin();
  display.setContrast(50);   // Ajustar el contraste de la pantalla

  // Inicializar salida serial
  Serial.begin(9600);

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

void screen(float voltage0, float voltage1, float voltage2, float voltage3){
  // Limpiar pantalla
  display.clearDisplay();

  // Mostrar el valor del sensor
  display.setCursor(0, 0);
  display.print("input0: ");
  display.print(voltage0);
  display.println("V");

  display.setCursor(0, 10);
  display.print("input1: ");
  display.print(voltage1);
  display.println("V");

  display.setCursor(0, 20);
  display.print("input2: ");
  display.print(voltage2);
  display.println("V");

  display.setCursor(0, 30);
  display.print("input3: ");
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

void loop() {
  int sensorValue0 = analogRead(A0);  // Leer tension en A0
  int sensorValue1 = analogRead(A1);  // Leer tension en A1
  int sensorValue2 = analogRead(A2);  // Leer tension en A2
  int sensorValue3 = analogRead(A3);  // Leer tension en A3
  float voltage0 = sensorValue0 * (5.0 / 1023.0); // Convertir a voltaje
  float voltage1 = sensorValue1 * (5.0 / 1023.0); // Convertir a voltaje
  float voltage2 = sensorValue2 * (5.0 / 1023.0); // Convertir a voltaje
  float voltage3 = sensorValue3 * (5.0 / 1023.0); // Convertir a voltaje

  screen(voltage0, voltage1, voltage2, voltage3);
  serialOut(voltage0, voltage1, voltage2, voltage3);
  
  // Esperar 500ms
  delay(500);
}
