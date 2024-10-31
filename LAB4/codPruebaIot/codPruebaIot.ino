void setup() {
  // Iniciar comunicación serial
  Serial.begin(9600);
}

void serialOut(float x, float y, float z, float voltage) {
  // Enviar los valores de X, Y, Z y voltaje al puerto serial
  Serial.print("X: ");
  Serial.println(x);

  Serial.print("Y: ");
  Serial.println(y);

  Serial.print("Z: ");
  Serial.println(z);

  Serial.print("Battery: ");
  Serial.println(voltage);

  Serial.println("-------------------");  // Separador entre lecturas
}

void loop() {
  // Generar valores aleatorios para pruebas
  float x = random(-100, 100) / 10.0;  // Valores entre -10.0 y 10.0
  float y = random(-100, 100) / 10.0;  // Valores entre -10.0 y 10.0
  float z = random(-100, 100) / 10.0;  // Valores entre -10.0 y 10.0
  float voltage = random(60, 90) / 10.0;  // Valores entre 6.0 y 9.0

  // Enviar valores por serial
  serialOut(x, y, z, voltage);

  // Esperar 500ms
  delay(500);
}
