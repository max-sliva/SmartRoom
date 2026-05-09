#include "CustomSerial.h"

CustomSerial* mySerial2;
const char message[15] = {'T','h','i','s','I','s','T','h','e','S','e','r','i','a','l'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);
  while (!Serial1);
  mySerial2 = new CustomSerial(&Serial1);
  mySerial2->onRequest(function);
  
}

void function(uint8_t comma, uint8_t length) {
  mySerial2->setDataElem(length, 0);
  for (uint8_t i = 0; i < length;i++) {
    mySerial2->setDataElem(message[i], i+1);
  }
  mySerial2->sendReceive(comma,16);
  mySerial2->resetDataArray();
  Serial.println(length);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial2->serialListener();
}
