#include "CustomSerial.h"

CustomSerial* mySerial1;
char buffer[256];
char inChar;
uint8_t bufCount = 0, i = 0;
boolean readyToWrite = false, writed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.print("|USB Serial begin|");
  Serial1.begin(74880);
  while (!Serial1)
    ;
  Serial.println("|Serial1 begin|");
  mySerial1 = new CustomSerial(&Serial1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (readyToWrite == false) {
    while (Serial.available() > 0) {
      inChar = static_cast<char>(Serial.read());
      if (inChar == '\n' || inChar == '\r') {
        if (bufCount > 0) {
          readyToWrite = true;
          break;
        }
      } else {
        buffer[bufCount++] = inChar;
      }
    }
  } else {
    while (i < bufCount) {
      mySerial1->setDataElem(buffer[i], i % 32);
      buffer[i++] = 0;
      writed = true;
      if ((i % 32) == 0) {
        writed = false;
        mySerial1->sendPackageExtra(0, 32);
        mySerial1->resetDataArray();
      }
    }
    if (writed) {
      writed = false;
      mySerial1->sendPackageExtra(0,i % 32);
      mySerial1->resetDataArray();
    }
    i = 0;
    bufCount = 0;
    readyToWrite = false;
  }
}
