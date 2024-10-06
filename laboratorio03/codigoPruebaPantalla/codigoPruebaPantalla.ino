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
static int contador = 0;

  // Limpiar pantalla
  display.clearDisplay();

  // Escribir el contador
  display.setCursor(0, 0);
  display.print("Contador: ");
  display.print(contador);

  // Mostrar en pantalla
  display.display();

  // Incrementar contador
  contador++;

  // Esperar 1 segundo
  delay(1000);
}

void loop() {
  int sensorValue = analogRead(A0);  // Leer un sensor en A0
  float voltage = sensorValue * (5.0 / 1023.0); // Convertir a voltaje

  // Limpiar pantalla
  display.clearDisplay();

  // Mostrar el valor del sensor
  display.setCursor(0, 0);
  display.print("Voltaje: ");
  display.print(voltage);
  display.println("V");

  // Mostrar en pantalla
  display.display();

  // Esperar 500ms
  delay(500);
}
