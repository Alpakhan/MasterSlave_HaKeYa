#include <Wire.h>

#define LED_PIN_RED 5
#define LED_PIN_BLUE 6

bool ledRedState = false;
bool ledBlueState = false;

void I2C_RxHandler(int numBytes) {
  int index = 0;
  byte receivedByte;
  
  while (Wire.available()) {
    receivedByte = Wire.read(); // Lese das nächste verfügbare Byte
    if (index == 2) {           // Dritter Byte (Index 2, da Indexierung bei 0 beginnt)
      ledRedState = receivedByte != 0;  // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
      Serial.print("Rote LED ist ");
      Serial.println(ledRedState ? "EIN" : "AUS");
    } else if (index == 3) {    // Vierter Byte (Index 3)
      ledBlueState = receivedByte != 0; // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
      Serial.print("Blaue LED ist ");
      Serial.println(ledBlueState ? "EIN" : "AUS");
    } else if (index == 4) {    // Fünfter Byte (Index 4)
      // Schalte beide LEDs ein, unabhängig vom empfangenen Byte
      ledRedState = true;
      ledBlueState = true;
      Serial.println("Beide LEDs sind EIN");
    }
    index++;  // Erhöhe den Index für das nächste Byte
  }
}

void setup() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  Wire.begin(0x55);             // I2C als Slave mit Adresse 0x55 initialisieren
  Wire.onReceive(I2C_RxHandler);
  Serial.begin(9600);           // Beginne Serial Kommunikation
}

void loop() {
  digitalWrite(LED_PIN_RED, ledRedState ? HIGH : LOW);   // Steuere rote LED (Ein oder Aus)
  digitalWrite(LED_PIN_BLUE, ledBlueState ? HIGH : LOW); // Steuere blaue LED (Ein oder Aus)
  delay(100);
}
