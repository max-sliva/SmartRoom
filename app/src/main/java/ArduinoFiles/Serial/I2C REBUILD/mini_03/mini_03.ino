/**
  SECONDARY CONTROLLER: ISKRA ARDUINO MINI
  INCLUDES:
  LEDHANDLER, LOCKHANDLER, CUSTOMSERIAL
*/

#include "LedHandler.h"
#include "LockHandler.h"
#include "CustomSerial.h"

#define NUMBEROFLEDS 2

char keymap[3][4] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' }
};
Keypad* myKeypad;
uint8_t ledPins[NUMBEROFLEDS] = { 10, 11 };


LedHandler myLeds(NUMBEROFLEDS, ledPins);
LockHandler myLocker(13, A0, new uint8_t[2]{ A4, A5 },
                     myKeypad, '12340000', 4, keymap[2][3]);
CustomSerial mySerial(&Serial);


void packageHandler(uint8_t comma, uint8_t data) {
  switch (comma) {
    case 0:
      myLeds.setValueAll(data, 1000);
      break;
    default:
      break;
  }
}

void packageExtraHandler(uint8_t comma, uint8_t length) {
  switch (comma) {
    case 0:
      for (uint8_t i = 0; i < length; i++) {
        myLeds.setValue(mySerial.getDataElem(i), 100);
      }
      break;
    case 1:
      myLeds.setValue(mySerial.getDataElem(0), mySerial.getDataElem(1), 1000);
      break;
    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(115200);  // CUSTOMSERIAL TO ARD MEGA
  while (!Serial);  // MUSTHAVE
  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  // INIT KEYPAD
  myKeypad = new Keypad(makeKeymap(keymap),
                        new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);

  mySerial.sendPackage(0, 0);  // SENDING REQUEST TO GET LED VALUE IF MEGA ACTIVE
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.serialListen();
  myLocker.lockListen();
}
