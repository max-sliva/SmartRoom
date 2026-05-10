#include "CustomSerial.h"

CustomSerial* mySerial1;
char buffer[16];
uint8_t counter = 1;
boolean state = true;
boolean state2 = true;
uint32_t time_ms;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);
  while (!Serial1);
  mySerial1 = new CustomSerial(&Serial1);
  mySerial1->onReceive(function);
  buffer[16] = 0;

  time_ms = millis();
}

void function(uint8_t comma, uint8_t length) {
  for (uint8_t i = 0; i < counter; i++) {
    buffer[i] = mySerial1->getDataElem(i+1);
  }
  Serial.println(buffer);
  state = true;
  if (++counter >= 16) {
    counter = 1;
    for (uint8_t i = 0; i < 16; i++) {
      buffer[i] = 0;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (state) {
    mySerial1->sendRequest(0, counter);
    time_ms = millis();
    state = false;
    delay(500);
  } else {
    mySerial1->serialListener();
    // if ((millis() - time_ms) > 10000) {
    //   state = true;
    //   Serial.print("|TIMEOUT|");
    //   time_ms = millis();
    // }
  }
}
