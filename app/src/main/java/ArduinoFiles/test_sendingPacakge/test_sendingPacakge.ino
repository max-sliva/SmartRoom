#include "CustomSerial.h"

CustomSerial mySerial(&Serial);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(115200);
  while (!Serial);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.sendPackage(0,0);
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
  delay(900);
}
