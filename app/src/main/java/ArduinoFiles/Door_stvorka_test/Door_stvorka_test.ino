/**
 * TEST STVORKAELEMENT FOR DOOR (had some problem in June 2026)
 */

#include "StvorkaElement.h"

StvorkaElement myDoor(5, 6, A5);
int button_pin = 2;
boolean but_state = false, door_state = false;
//343 157
void setup() {
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  myDoor.setBoundaries(250, 750);
  Serial.begin(9600);
  while (!Serial)
    ;
}

void loop() {
  // put your main code here, to run repeatedly:
  boolean input = digitalRead(button_pin);
  if ((input == LOW) && (!but_state)) {
    digitalWrite(13, HIGH);
    but_state = true;
  } else {
    if ((input == HIGH) && but_state) {
      door_state = !door_state;
      if (door_state) {
        Serial.println("open");
        myDoor.openStvorka();
      } else {
        Serial.println("close");
        myDoor.closeStvorka();
      }
      digitalWrite(13, LOW);
      but_state = false;
    }
  }
}
