#include <Wire.h>
#include "LedHandler.h"

uint8_t dataContr[2] = {8,2};

LedHandler* my_leds;

void receiveEvent() {
  uint8_t dataFunc[3];
  uint8_t count = 0;
  while(Wire.available()) {   // Пока есть данные
    dataFunc[count] = Wire.read(); // Получить 1 байт
    count++;
    if (count > 3) break;
  }
  switch (dataFunc[0]) {
    case 0:
      dataContr[1] = dataFunc[1];
      break;
    case 1:
      my_leds->setValueAll(dataFunc[1],1000);
      break;
    case 2:
      my_leds->setValue(dataFunc[1],dataFunc[2],1000);
      break;
  }
}

void requestEvent() {
  switch (dataContr[1]) {
    case 2:
      for (uint8_t i = 0; i < my_leds->getNumberOfConnetcion(); i++) {
        Wire.write(my_leds->getValue(i));
      }
      break;
  }
}

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  my_leds = new LedHandler(2, new uint8_t[2]{10,11});
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}
