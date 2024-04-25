/*
* LAB Name: Arduino I2C Slave(Rx)
* Author: Khaled Magdy
* For More Info Visit: www.DeepBlueMbedded.com
*/
#include <Wire.h>
 
#define LED_PIN_RED 5
#define LED_PIN_BLUE 6

byte ledredValue = 0;
byte ledblueValue = 0;

byte RxByte;
 
void I2C_RxHandler(int numBytes) {
  int index = 0;
  // Serial.println("Hallo");
  // Serial.println(RxByte);
  while (Wire.available()) {
    if (index == 4) {
      ledredValue = Wire.read();
      index++;
    } else if (index == 3) {
      ledblueValue = Wire.read();
      index++;
    } else if (index ==5){
      ledblueValue = Wire.read();
      ledredValue = Wire.read();
      index++;
    }
  }
}
void setup() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  Wire.begin(0x55); // Initialize I2C (Slave Mode: address=0x08 )
  Wire.onReceive(I2C_RxHandler);
  Serial.println(RxByte);
}
 
void loop() {
  analogWrite(LED_PIN_RED, RxByte);
  analogWrite(LED_PIN_BLUE, RxByte);
  delay(100);
}
