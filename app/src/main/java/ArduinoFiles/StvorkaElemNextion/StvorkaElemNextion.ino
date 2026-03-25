#include "StvorkaElement.h"

StvorkaElement* stvorkas[2]; 

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  Serial.println("===> Serial was started <========================================");
  while (!Serial);
  for (int i = 0; i < 2; i++) {
    stvorkas[i] = new StvorkaElement(i*8);
    Serial.print("Stvorka inside Boundaries --> ");
    Serial.println(stvorkas[i]->valueInsideBoundaries(stvorkas[i]->updateValue()));
    Serial.println(stvorkas[i]->updateValue());
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(13,100);
  delay(100);
  digitalWrite(13,LOW);
  delay(900);
}
