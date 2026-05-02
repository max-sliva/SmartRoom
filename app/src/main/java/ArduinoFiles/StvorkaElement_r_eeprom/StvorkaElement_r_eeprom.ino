#include "StvorkaElement.h"

bool state1 = false, state2 = false;
StvorkaElement* stvorka;

void setup() {
  pinMode(A1, INPUT);
  pinMode(11, INPUT_PULLUP);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("==========================================================");
  stvorka = new StvorkaElement(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool but = digitalRead(11);
  if (but == LOW) state1 = true;
  if (but == HIGH && state1 == true) {
    uint8_t value = map(analogRead(A1),0,1023,0,255);
    Serial.println(value);
    stvorka->moveRelativelyTo(value);
    state1 = false;
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
  }
  delay(50);
}
