// #include <Wire.h>

// #define LED_PIN_RED 5
// #define LED_PIN_BLUE 6

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
//   Wire.begin(0x55);             // I2C als Slave mit Adresse 0x55 initialisieren
//   Wire.onReceive(I2C_RxHandler);
//   Serial.begin(9600);           // Beginne Serial Kommunikation
// }

// void loop() {
//   digitalWrite(LED_PIN_RED, ledRedState ? HIGH : LOW);   // Steuere rote LED (Ein oder Aus)
//   digitalWrite(LED_PIN_BLUE, ledBlueState ? HIGH : LOW); // Steuere blaue LED (Ein oder Aus)
//   delay(100);
// }
#include <Wire.h>

// Definiere die Slave-Adresse
#define SLAVE_ADDRESS 0x55

// Beispiel Daten, die gesendet werden sollen
byte dataToSend[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int dataIndex = 0;

void setup() {
  // Starte die I2C-Bibliothek als Slave
  Wire.begin(SLAVE_ADDRESS);
  
  // Registriere den Request-Event-Handler
  Wire.onRequest(requestEvent);
  
  // Starte die serielle Kommunikation für Debugging
  Serial.begin(9600);
}

void loop() {
  // Hier könnte weitere Logik eingebaut werden.
  delay(100); // Einfache Verzögerung in der Schleife
}

// Diese Funktion wird aufgerufen, wenn der Master Daten anfordert
void requestEvent() {
  // Sende das nächste Byte des Datenarrays
  Wire.write(dataToSend[dataIndex]);
  
  // Aktualisiere den Index für das nächste zu sendende Byte
  dataIndex = (dataIndex + 1) % sizeof(dataToSend);
  
  // Optional: Ausgabe auf den Serial Monitor für Debugging
  Serial.print("Gesendet: ");
  Serial.println(dataToSend[dataIndex]);
}
