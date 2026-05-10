#include "CustomSerial.h"

CustomSerial* mySerial2;
uint32_t ms;
boolean state = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.print("|USB Serial begin|");
  Serial1.begin(9600);
  while (!Serial1)
    ;
  Serial.println("|Serial1 begin|");
  mySerial2 = new CustomSerial(&Serial1);
  mySerial2->onReceive(function);
  mySerial2->onRequest(function2);
  ms = millis();
  pinMode(13,OUTPUT);
}

void function2(uint8_t comma, uint8_t length) {
  digitalWrite(13,HIGH);
  delay(50);
  digitalWrite(13,LOW);
  delay(150);
}

void function(uint8_t comma, uint8_t length) {
  Serial.print(length);
  Serial.print(" ==> ");
  for (uint8_t i = 0; i < length; i++) {
    Serial.write(mySerial2->getDataElem(i));
  }
  Serial.println();
  mySerial2->resetDataArray();
  state = false;
}

void loop() {
  mySerial2->serialListener();
  if (state == false) {
    state = true;
    mySerial2->sendRequest(0, 0);
  }
}
