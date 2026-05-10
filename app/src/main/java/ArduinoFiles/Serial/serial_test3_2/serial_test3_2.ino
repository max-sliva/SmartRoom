#include "CustomSerial.h"

CustomSerial* mySerial2;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.print("|USB Serial begin|");
  Serial1.begin(74880);
  while (!Serial1);
  Serial.println("|Serial1 begin|");
  mySerial2 = new CustomSerial(&Serial1);
  mySerial2->onPackageExtra(function);
}

void function(uint8_t comma, uint8_t length) {
  for (uint8_t i = 0; i < length;i++) {
    Serial.write(mySerial2->getDataElem(i));
  }
  Serial.print(" ==> ");
  Serial.println(length);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial2->serialListener();
}
