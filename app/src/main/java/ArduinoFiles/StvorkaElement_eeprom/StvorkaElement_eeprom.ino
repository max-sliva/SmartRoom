#include "StvorkaElement.h"

StvorkaElement stvorka(2,3,A5);

void setup() {
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("=====================================");
  Serial.println("Start findBoundaries? [y/n]");
  while (!(Serial.available()>0)) {
    delay(100);
  }
  if (Serial.read()!='y') {
    stvorka.findBoundaries(150);
  }
  Serial.println("Write Bytes? [y/n]");
  while (!(Serial.available()>0)) {
    delay(100);
  }
  if (Serial.read()!='y') {
    stvorka.writeBytesToEEPROM(16);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(13,100);
  delay(100);
  digitalWrite(13,LOW);
  delay(900);
}
