#include <Wire.h>
#include <Arduino.h>

#define LED_PIN_RED 13
#define LED_PIN_BLUE 12

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

void sendData() {
  Wire.write(ledRedState ? 1 : 0);
  Wire.write(ledBlueState ? 1 : 0);
}

void setup() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  Wire.begin(0x09);             // I2C als Slave mit Adresse 0x55 initialisieren
  Wire.onReceive(I2C_RxHandler);
  Serial.begin(9600);           // Beginne Serial Kommunikation
}

void loop() {
  digitalWrite(LED_PIN_RED, ledRedState ? HIGH : LOW);   // Steuere rote LED (Ein oder Aus)
  digitalWrite(LED_PIN_BLUE, ledBlueState ? HIGH : LOW); // Steuere blaue LED (Ein oder Aus)
  delay(100);
}

// #include <Wire.h>

// #define SLAVE_ADDRESS 0x04  // I2C Adresse des Arduino-Slaves

// void requestEvent() {
//   byte dataToSend = 0x55;               // Beispiel Daten zum Senden
//   Wire.write(dataToSend);               // Sendet ein Byte an den Master
//   Serial.println("Daten gesendet");     // Gibt eine Nachricht auf dem Serial Monitor aus
// }

// void setup() {
//   Wire.begin(SLAVE_ADDRESS);            // Startet den I2C-Bus als Slave
//   Wire.onRequest(requestEvent);         // Registriert die onRequest-Event-Handler-Funktion
//   Serial.begin(9600);                   // Startet die serielle Kommunikation für Debugging
// }

// void loop() {
//   // Hauptprogrammschleife des Arduino, hier wird nichts benötigt
//   delay(100);
// }

// Diese Funktion wird ausgeführt, wenn der Master Daten anfordert

// #include <Arduino.h>

// // Definiere die Pin-Nummern für die LEDs
// const int ledPin1 = 12;  
// const int ledPin2 = 13;
// // #define LED_PIN_RED 5
// // #define LED_PIN_BLUE 6
// void setup() {
//   // Setze die LED Pins als Ausgang
//   pinMode(ledPin1, OUTPUT);
//   pinMode(ledPin2, OUTPUT);
// }

// void loop() {
//   // Schalte LED 1 an
//   digitalWrite(ledPin1, HIGH); 
//   digitalWrite(ledPin2, LOW);  
//   delay(1000);                    // Warte 1 Sekunde

//   // Schalte LED 1 aus und LED 2 an
//   digitalWrite(ledPin1, LOW);    
//   digitalWrite(ledPin2, HIGH);    
//   delay(1000);                    // Warte 1 Sekunde
    
//   delay(1000);                    // Warte 1 Sekunde
// }
