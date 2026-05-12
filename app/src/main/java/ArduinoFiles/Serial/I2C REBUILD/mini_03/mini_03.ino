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
    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);  // CUSTOMSERIAL TO ARD MEGA
  while (!Serial);  // MUSTHAVE
  Serial.println("Serial begin");
  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  // INIT KEYPAD
  Keypad* myKeypad = new Keypad(makeKeymap(keymap),
                        new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);
  myLocker = new LockHandler(13, A0, new uint8_t[2]{ A4, A5 },
                     myKeypad, "1234", 4, keymap[2][3]);
  pinMode(A7,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //mySerial.serialListen();
  myLocker->lockListen();
  // if ((extPower == false)&&(analogRead(A7)>150)) {
  //   extPower = true;
  //   Serial.begin(115200);
  //   while (!Serial);
  //   mySerial.sendPackage(0, 0); // SENDING REQUEST TO GET LED VALUE IF MEGA ACTIVE

  // }
  // if ((extPower == true)&&(analogRead(A7)<150)) {
  //   extPower = false;
  //   Serial.end();
  // }
}
