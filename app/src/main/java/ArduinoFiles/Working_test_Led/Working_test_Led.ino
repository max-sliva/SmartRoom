#include "EasyNextionLibrary.h"
#include "LedElement.h"

EasyNex myNex(Serial3);  // Using Hardware Serial
LedElement led1(8, "h1"), led2(9, "h2");

void setup() {
  Serial.begin(9600);  // Must match Nextion baud rate
  myNex.begin(9600);
}

void loop() {
  myNex.NextionListen();
}

void trigger0() {
  led1.updateValueNextion(&myNex);
  led2.updateValueNextion(&myNex);
  led1.updateLed();
  led2.updateLed();
}