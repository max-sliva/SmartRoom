#include "StvorkaElement.h"

bool state1 = false, state2 = false;

void setup() {
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while (!Serial);
}

StvorkaElement stvorka(0);

void loop() {
  // put your main code here, to run repeatedly:
  bool but = digitalRead(11);
  if (but == LOW) state1 = true;
  if (but == HIGH && state1 == true) {
    stvorka.openSrvorka();
    state1 = false;
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
  }
  but = digitalRead(12);
  if (but == LOW) state2 = true;
  if (but == HIGH && state2 == true) {
    stvorka.closeSrvorka();
    state2 = false;
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);  
  }
  delay(50);
}
