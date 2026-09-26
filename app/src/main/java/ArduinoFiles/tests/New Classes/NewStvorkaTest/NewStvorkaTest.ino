#include "StvorkaElement.h"

StvorkaElement stv1 = StvorkaElement(4,5,A4);

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial Active!");
}

void loop() {
  // put your main code here, to run repeatedly:
  stv1.findBoundaries(200);
  analogWrite(13,100);
  delay(250);
  digitalWrite(13, LOW);
  delay(1750);
}
