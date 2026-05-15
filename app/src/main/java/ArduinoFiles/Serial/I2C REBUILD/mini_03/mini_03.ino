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
uint8_t ledPins[NUMBEROFLEDS] = { 10, 11 };
boolean extPower = false;

LedHandler myLeds(NUMBEROFLEDS, ledPins);
LockHandler* myLocker;
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
    case 2:
      char buffer[8];
      for (uint8_t i = 0; i < length;i++) {
        buffer[i] = mySerial.getDataElem(i);
      }
      mySerial.sendPackage(1,myLocker->checkHashPassword(buffer));
    default:
      break;
  }
} 

void sendLockState() {
  mySerial.resetDataArray();
  mySerial.setDataElem(myLocker->getLockedState(), 0);
  mySerial.setDataElem(myLocker->getStateWritePass(), 1);
  mySerial.sendPackageExtra(0,2);
}

void setup() {
  // put your setup code here, to run once
  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  mySerial.begin(115200);
  while (!Serial);
  mySerial.sendPackage(0, 0);

  // INIT KEYPAD
  Keypad* myKeypad = new Keypad(makeKeymap(keymap),
                        new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);
  myLocker = new LockHandler(13, A0, new uint8_t[2]{ A4, A5 },
                     myKeypad, "1234", 4, keymap[2][3]);
  myLocker->onAction(sendLockState,LockHandler::GRANTACCESS);
  myLocker->onAction(sendLockState,LockHandler::REVOKEACCESS);
  myLocker->onAction(sendLockState,LockHandler::REQUESTACCESS);
  myLocker->onAction(sendLockState,LockHandler::REQUESTABORT);
  myLocker->onAction(sendLockState,LockHandler::TIMEOUT);
  pinMode(A7,INPUT);    // analog input for checking state of activation of ard mega
  pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.serialListen();
  myLocker->lockListen();
}
