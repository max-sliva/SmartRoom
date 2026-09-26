#include "Blinds.h"

const int up = 0;
const int down = 40;
int value;
static int choose_value = 0;
static Blinds blinds(A2, A3, 9, 10, down, up, 128);
boolean but_state = LOW;

void setup() {
  pinMode(3,OUTPUT);
  pinMode(1,INPUT_PULLUP);
  pinMode(A5,INPUT);

  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buffer = analogRead(A5);
  if (choose_value != buffer) {
    choose_value = buffer;
    analogWrite(3,map(buffer,0,1023,0,255));
  }
  if (digitalRead(1) == LOW && !but_state) {
    but_state = HIGH;
    digitalWrite(3,LOW);
  }
  if (digitalRead(1) == HIGH && but_state) {
    blinds.moveToValue(map(buffer,0,1023,up,down));
    but_state = LOW;
    digitalWrite(3,map(buffer,0,1023,0,255));
  }
}
