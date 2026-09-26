#include "StvorkaElement.h"

StvorkaElement stvorka(6,7,A0);
bool state1 = false, state2 = false;

void setup() {
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  stvorka.findBoundaries(200,150);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool but = digitalRead(11);
  if (but == LOW) state1 = true;
  if (but == HIGH && state1 == true) {
    stvorka.openSrvorka(150);
    state1 = false;
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    delay(800);  
  }
  but = digitalRead(12);
  if (but == LOW) state2 = true;
  if (but == HIGH && state2 == true) {
    stvorka.closeSrvorka(150);
    state2 = false;
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    delay(800);  
  }
  delay(50);
}
