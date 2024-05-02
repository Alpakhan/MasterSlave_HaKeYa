
#include <Arduino.h>
#include <Wire.h>

#define LED_PIN_RED 13
#define LED_PIN_BLUE 12

void I2C_RxHandler(int numBytes) {
  while (Wire.available()) {
    byte receivedByte = Wire.read(); // Lese das nächste verfügbare Byte

        // Gib den empfangenen Befehl aus
    Serial.print("Empfangenes Byte: 0x");
    Serial.println(receivedByte, HEX); // HEX für hexadezimale Darstellung
    Serial.print("Binär: ");
    Serial.println(receivedByte, BIN); // BIN für binäre Darstellung

    digitalWrite(LED_PIN_RED, receivedByte & 0x03 ? HIGH : LOW);  // Setze die weiße LED (Bit 0)
    digitalWrite(LED_PIN_BLUE, receivedByte & 0x04 ? HIGH : LOW);  // Setze die grüne LED (Bit 1)
    digitalWrite(LED_PIN_RED & LED_PIN_BLUE, receivedByte & 0x05 ? HIGH : LOW);
    Serial.print("Rote LED ist ");
    Serial.println(receivedByte & 0x03 ? "EIN" : "AUS");
    Serial.print("Blaue LED ist ");
    Serial.println(receivedByte & 0x04 ? "EIN" : "AUS");
    Serial.println("Beide sind");
    Serial.println(receivedByte & 0x05 ? "EIN" : "AUS");
  }
}
void I2C_RxHandler(int numBytes) {
  while (Wire.available()) {
    byte receivedByte = Wire.read(); // Lese das nächste verfügbare Byte

    // Ausgabe des empfangenen Befehls
    Serial.print("Empfangenes Byte: 0x");
    Serial.println(receivedByte, HEX); // HEX für hexadezimale Darstellung
    Serial.print("Binär: ");
    Serial.println(receivedByte, BIN); // BIN für binäre Darstellung

    // Steuerung der roten LED (Bit 0)
    digitalWrite(LED_PIN_RED, receivedByte & 0x03 ? HIGH : LOW);
    // Steuerung der blauen LED (Bit 1)
    digitalWrite(LED_PIN_BLUE, receivedByte & 0x04 ? HIGH : LOW);

    // Gleichzeitige Steuerung beider LEDs (Bit 2)
    if (receivedByte & 0x06) {
      digitalWrite(LED_PIN_RED, HIGH);
      digitalWrite(LED_PIN_BLUE, HIGH);
    } else {
      // Wenn Bit 2 nicht gesetzt ist, behalten die anderen Bits ihre Kontrolle.
      digitalWrite(LED_PIN_RED, receivedByte & 0x03 ? HIGH : LOW);
      digitalWrite(LED_PIN_BLUE, receivedByte & 0x04 ? HIGH : LOW);
    }

    Serial.print("Rote LED ist ");
    Serial.println(digitalRead(LED_PIN_RED) ? "EIN" : "AUS");
    Serial.print("Blaue LED ist ");
    Serial.println(digitalRead(LED_PIN_BLUE) ? "EIN" : "AUS");
  }
}

void I2C_TxHandler() {
  byte status = 0;
  status |= digitalRead(LED_PIN_RED) << 0;  // Bit 0 für rote LED
  status |= digitalRead(LED_PIN_BLUE) << 1; // Bit 1 für blaue LED

  Wire.write(status); // Sende den Status zurück zum Master
}


void setup() {
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  Wire.begin(0x09);  // I2C als Slave mit Adresse 0x08
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);
  Serial.begin(9600);  // Beginne Serial Kommunikation
}

void loop() {
  // Hier ist keine Aktualisierung notwendig, da die LEDs im Interrupt-Handler gesetzt werden
  delay(100);
}


