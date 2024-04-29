// #include <Arduino.h>
// #include <Wire.h>


// #define LED_PIN_WHITE 12
// #define LED_PIN_GREEN 13

// bool ledWhiteState = false;
// bool ledGreenState = false;

// void I2C_RxHandler(int numBytes) {
//   int index = 0;
//   byte receivedByte;
  
//   while (Wire.available()) {
//     receivedByte = Wire.read(); // Lese das nächste verfügbare Byte
//     if (index == 0) {           // Erster Byte (Index 0)
//       ledWhiteState = receivedByte != 0;  // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
//       Serial.print("Weisse LED ist ");
//       Serial.println(ledWhiteState ? "EIN" : "AUS");
//     } else if (index == 1) {    // Zweiter Byte (Index 1)
//       ledGreenState = receivedByte != 0; // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
//       Serial.print("Gruene LED ist ");
//       Serial.println(ledGreenState ? "EIN" : "AUS");
//     } else if (index == 5) {    // Fünfter Byte (Index 4)
//       // Schalte beide LEDs ein oder aus, abhängig vom empfangenen Byte
//       bool newState = receivedByte != 0;
//       ledWhiteState = newState;
//       ledGreenState = newState;
//       Serial.print("Beide LEDs sind ");
//       Serial.println(newState ? "EIN" : "AUS");
//     }
//     index++;  // Erhöhe den Index für das nächste Byte
//   }
// }
// void sendData() {
//   Wire.write(ledWhiteState ? 1 : 0);
//   Wire.write(ledGreenState ? 1 : 0);
// }
// void setup() {
//   pinMode(LED_PIN_WHITE, OUTPUT);
//   pinMode(LED_PIN_GREEN, OUTPUT);
//   Wire.begin(0x08);             // I2C als Slave mit einer anderen Adresse, z.B. 0x56
//   Wire.onReceive(I2C_RxHandler);
//   Serial.begin(9600);           // Beginne Serial Kommunikation
// }

// void loop() {
//   digitalWrite(LED_PIN_WHITE, ledWhiteState ? HIGH : LOW); // Steuere weiße LED (Ein oder Aus)
//   digitalWrite(LED_PIN_GREEN, ledGreenState ? HIGH : LOW);     // Steuere rote LED (Ein oder Aus)
//   delay(100);
// }
#include <Arduino.h>
#include <Wire.h>

#define LED_PIN_WHITE 12
#define LED_PIN_GREEN 13

void I2C_RxHandler(int numBytes) {
  while (Wire.available()) {
    byte receivedByte = Wire.read(); // Lese das nächste verfügbare Byte

        // Gib den empfangenen Befehl aus
    Serial.print("Empfangenes Byte: 0x");
    Serial.println(receivedByte, HEX); // HEX für hexadezimale Darstellung
    Serial.print("Binär: ");
    Serial.println(receivedByte, BIN); // BIN für binäre Darstellung
    
    digitalWrite(LED_PIN_WHITE, receivedByte & 0x01 ? HIGH : LOW);  // Setze die weiße LED (Bit 0)
    digitalWrite(LED_PIN_GREEN, receivedByte & 0x02 ? HIGH : LOW);  // Setze die grüne LED (Bit 1)
    digitalWrite(LED_PIN_WHITE & LED_PIN_GREEN, receivedByte & 0x05 ? HIGH : LOW);
    Serial.print("Weiße LED ist ");
    Serial.println(receivedByte & 0x01 ? "EIN" : "AUS");
    Serial.print("Grüne LED ist ");
    Serial.println(receivedByte & 0x02 ? "EIN" : "AUS");
    Serial.println("Beide sind");
    Serial.println(receivedByte & 0x05 ? "EIN" : "AUS");
  }
}

void setup() {
  pinMode(LED_PIN_WHITE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  Wire.begin(0x08);  // I2C als Slave mit Adresse 0x08
  Wire.onReceive(I2C_RxHandler);
  Serial.begin(9600);  // Beginne Serial Kommunikation
}

void loop() {
  // Hier ist keine Aktualisierung notwendig, da die LEDs im Interrupt-Handler gesetzt werden
  delay(100);
}