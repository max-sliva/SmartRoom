#include "StvorkaElement.h"

StvorkaElement stvorka(6,7,A0);

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  stvorka.findBoundaries(200,150);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}
