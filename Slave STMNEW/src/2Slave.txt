#include <Wire.h>

#define LED_PIN_WHITE 5
#define LED_PIN_Green 6

bool ledWhiteState = false;
bool ledGreenState = false;

void I2C_RxHandler(int numBytes) {
  int index = 0;
  byte receivedByte;
  
  while (Wire.available()) {
    receivedByte = Wire.read(); // Lese das nächste verfügbare Byte
    if (index == 0) {           // Erster Byte (Index 0)
      ledWhiteState = receivedByte != 0;  // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
      Serial.print("Weiße LED ist ");
      Serial.println(ledWhiteState ? "EIN" : "AUS");
    } else if (index == 1) {    // Zweiter Byte (Index 1)
      ledGreenState = receivedByte != 0; // Setze den Zustand auf HIGH, wenn das Byte nicht 0 ist
      Serial.print("Rote LED ist ");
      Serial.println(ledGreenState ? "EIN" : "AUS");
    } else if (index == 5) {    // Fünfter Byte (Index 4)
      // Schalte beide LEDs ein oder aus, abhängig vom empfangenen Byte
      bool newState = receivedByte != 0;
      ledWhiteState = newState;
      ledGreenState = newState;
      Serial.print("Beide LEDs sind ");
      Serial.println(newState ? "EIN" : "AUS");
    }
    index++;  // Erhöhe den Index für das nächste Byte
  }
}
void sendData() {
  Wire.write(ledWhiteState ? 1 : 0);
  Wire.write(ledGreenState ? 1 : 0);
}
void setup() {
  pinMode(LED_PIN_WHITE, OUTPUT);
  pinMode(LED_PIN_Green, OUTPUT);
  Wire.begin(0x56);             // I2C als Slave mit einer anderen Adresse, z.B. 0x56
  Wire.onReceive(I2C_RxHandler);
  Serial.begin(9600);           // Beginne Serial Kommunikation
}

void loop() {
  digitalWrite(LED_PIN_WHITE, ledWhiteState ? HIGH : LOW); // Steuere weiße LED (Ein oder Aus)
  digitalWrite(LED_PIN_Green, ledGreenState ? HIGH : LOW);     // Steuere rote LED (Ein oder Aus)
  delay(100);
}
