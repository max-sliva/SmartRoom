#include "StvorkaElement.h"

EasyNex myNex(Serial3);  // Using Hardware Serial
StvorkaElement* stvorkas[2]; 



void setup() {
  Serial.begin(9600); // Must match Nextion baud rate
  myNex.begin(9600);
  while (!Serial);
  Serial.println("===> Serial was started <========================================");
  for (int i = 0; i < 2; i++) {
    stvorkas[i] = new StvorkaElement(i*8);
  }
}

void loop() {
  myNex.NextionListen();
}

// Trigger that trun stvorkas to position from nextion page /w windows
void trigger1() {
  
}
//  TO DO Trigger that puts window stvorkas values in nextion page /w windows
void trigger2() {
  stvorkas[0].moveRelativelyToByte(myNex.readNumber("h1.val"));
  stvorkas[1].moveRelativelyToByte(myNex.readNumber("h2.val"));
}