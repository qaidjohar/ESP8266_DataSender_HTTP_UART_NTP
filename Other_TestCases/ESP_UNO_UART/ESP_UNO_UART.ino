#include <SoftwareSerial.h>

// Connect RX pin of ESP8266 with Pin 1 (tx) pin of Arduino Uno

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  if (Serial.available()) {
    Serial.write(Serial.read());
  }
}
