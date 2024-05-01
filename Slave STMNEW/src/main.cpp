//#include <Wire.h>
// #include <Arduino.h>

// #define LED_PIN_RED 13
// #define LED_PIN_BLUE 12

// bool ledRedState = false;
// bool ledBlueState = false;

// void I2C_RxHandler(int numBytes) {
//   int index = 0;
//   byte receivedByte;
  
//   while (Wire.available()) {
//     receivedByte = Wire.read(); // Lese das nächste verfügbare Byte
//     if (index == 2) {           // Dritter Byte (Index 2, da Indexierung bei 0 beginnt)
//       ledRedState = receivedByte != 0;  // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
//       Serial.print("Rote LED ist ");
//       Serial.println(ledRedState ? "EIN" : "AUS");
//     } else if (index == 3) {    // Vierter Byte (Index 3)
//       ledBlueState = receivedByte != 0; // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
//       Serial.print("Blaue LED ist ");
//       Serial.println(ledBlueState ? "EIN" : "AUS");
//     } else if (index == 4) {    // Fünfter Byte (Index 4)
//       // Schalte beide LEDs ein, unabhängig vom empfangenen Byte
//       ledRedState = true;
//       ledBlueState = true;
//       Serial.println("Beide LEDs sind EIN");
//     }
//     index++;  // Erhöhe den Index für das nächste Byte
//   }
// }

// void sendData() {
//   Wire.write(ledRedState ? 1 : 0);
//   Wire.write(ledBlueState ? 1 : 0);
// }

// void setup() {
//   pinMode(LED_PIN_RED, OUTPUT);
//   pinMode(LED_PIN_BLUE, OUTPUT);
//   Wire.begin(0x09);             // I2C als Slave mit Adresse 0x55 initialisieren
//   Wire.onReceive(I2C_RxHandler);
//   Serial.begin(9600);           // Beginne Serial Kommunikation
// }

// void loop() {
//   digitalWrite(LED_PIN_RED, ledRedState ? HIGH : LOW);   // Steuere rote LED (Ein oder Aus)
//   digitalWrite(LED_PIN_BLUE, ledBlueState ? HIGH : LOW); // Steuere blaue LED (Ein oder Aus)
//   delay(100);
// }

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
    Serial.println(receivedByte & 0x06 ? "EIN" : "AUS");
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
  Wire.begin(0x08);  // I2C als Slave mit Adresse 0x08
  Wire.onReceive(I2C_RxHandler);
  Wire.onRequest(I2C_TxHandler);
  Serial.begin(9600);  // Beginne Serial Kommunikation
}

void loop() {
  // Hier ist keine Aktualisierung notwendig, da die LEDs im Interrupt-Handler gesetzt werden
  delay(100);
}


