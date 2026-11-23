/**
  SECONDARY CONTROLLER: ISKRA ARDUINO MINI
  INCLUDES:
  LEDHANDLER, LOCKHANDLER, CUSTOMSERIAL, STVORKAELEMENT
*/

#define SMARTHOUSE_DEVICE_MINI
#include <library_for_sh.h>

char keymap[3][4] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' }
};
boolean extPower = false;

CustomSerial mySerial(&Serial);

uint8_t mainLedPins[2] = {10,11}, 
        rows[3] = {4,9,12},
        columns[4] = {7,3,2,8},
        lockLeds[2] = {A2,A3};

LedHandler myLeds(2, mainLedPins);
StvorkaElement myDoor(5,6,A6);
Keypad lockNumpad(makeKeymap(keymap),
                  rows, columns,
                  3, 4);
LockHandler myLocker(13, A0,
                    lockLeds,
                    &lockNumpad,
                    "1234", 4,
                    '#');

void packageHandler(uint8_t comma, uint8_t data) {
  switch (comma) {
    case 0:
      if (myLeds.allEqual()) {
        myLeds.setValueAllSync(data, 1000);
      } else {
        myLeds.setValueAllAsync(data, 1000);
      }
      break;
    case 10:
      if (data == 1) myDoor.openStvorka();
      else myDoor.closeStvorka();
      break;
    case 11:
      if (data == 1) myLocker.setLockedState(true);
      else myLocker.setLockedState(false);
      break;
    case 12:
      if (data == 1) myLocker.grantAccess();
      else myLocker.accessDenied();
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

void grantAccessFunc() {
  myDoor.openStvorka();
  mySerial.sendPackage(9, 1);
}

void revokeAccessFunc() {
  myDoor.closeStvorka();
  mySerial.sendPackage(9, 0);
}

void requestAccess() {
  mySerial.sendPackage(10, 0);
}

void requestAbort() {
  mySerial.sendPackage(11, 0);
}

void setup() {
  //MYDOOR STVORKAELEMENT BOUNDARIES
  myDoor.setBoundaries(150, 495);
  myDoor.closeStvorka();
  // CUSTOM SERIAL
  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  mySerial.begin(115200);
  while (!Serial);
  //mySerial.sendPackage(0, 0);
  // Sending door & locker states
  mySerial.setDataElem(1, 0);
  mySerial.setDataElem(1, 1);
  mySerial.setDataElem(0, 2);
  mySerial.sendPackageExtra(0, 3);
  // setting action to LockHandler
  myLocker.onAction(grantAccessFunc, LockHandler::GRANTACCESS);
  myLocker.onAction(revokeAccessFunc, LockHandler::REVOKEACCESS);
  myLocker.onAction(requestAccess, LockHandler::REQUESTACCESS);
  myLocker.onAction(requestAbort, LockHandler::REQUESTABORT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.serialListen();
  myLocker.lockListen();
}
