#include <Wire.h>
#include <Keypad.h>
#include "LedHandler.h"

uint8_t dataContr[2] = {8,0};
const uint8_t numRows = 4; //кол-во строк
const uint8_t numCols = 3; //кол-во столбцов
char buffer;
char keymap[numRows][numCols] = 
{
  ['1','2','3'],
  ['4','5','6'],
  ['7','8','9'],
  ['*','0','#']
};
uint8_t rowPins[numRows] = {1,2,3,4};
uint8_t colPins[numCols] = {7,8,9};
uint8_t buttonOut = A2, buttonIn = 13;

LedHandler* my_leds;
Keypad* my_keypad;

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

  pinMode(buttonIn,INPUT);
  pinMode(buttonOUT,OUTPUT);

  my_leds = new LedHandler(2, new uint8_t[2]{10,11});
  my_keypad = new Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
}

void loop() {
  if (digitalRead(buttonIn) == HIGH) {
    if 
  }
  buffer = myKeypad->getKey(); 
  if (buffer != NO_KEY) {
    if (dataContr[1] == 0) {
      dataContr[1] = 1;

    }
  }
}
