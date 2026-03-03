#include "StvorkaElement.h"

StvorkaElement stvorka(6,7,A0);

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  stvorka.findBoundaries(200);
  stvorka.writeBytesToEEPROM(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  delay(950);
  digitalWrite(13,LOW);
  delay(50);
}
