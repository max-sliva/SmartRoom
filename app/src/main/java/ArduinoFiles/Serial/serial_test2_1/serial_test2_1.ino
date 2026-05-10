#include "CustomSerial.h"

char buffer[32];
char inChar;
uint8_t bufCount = 0;
boolean readyToSend = false;
CustomSerial* mySerial1;
uint16_t counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.print("|USB Serial begin|");
  Serial1.begin(9600);
  while (!Serial1);
  Serial.println("|Serial1 begin|");
  mySerial1 = new CustomSerial(&Serial1);
  mySerial1->onRequest(function);
}

void function(uint8_t comma, uint8_t length) {
  if (readyToSend) {
    for (uint8_t i = 0; i < bufCount; i++) {
      mySerial1->setDataElem(buffer[i],i);
      buffer[i] = 0;
    }
    mySerial1->sendReceive(1,bufCount);
    mySerial1->resetDataArray();
    bufCount = 0;
    readyToSend = false;
  } else {
    mySerial1->sendRequest(0,counter++);
    if (counter > 32) {
      counter = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial1->serialListener();
  if (readyToSend == false) {
    while (Serial.available() > 0) {
      inChar = static_cast<char>(Serial.read());
      if (inChar == '\n' || inChar == '\r') {
        if (bufCount > 0) {
          readyToSend = true;
          break;
        }
      } else {
        buffer[bufCount++] = inChar;
        if (bufCount > 32) {
          readyToSend = true;
          break;
        }
      }
    }
  }
}
