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
uint8_t ledPins[2] = { 10, 11 };
boolean extPower = false;

LedHandler myLeds(2, ledPins);
LockHandler* myLocker;
CustomSerial mySerial(&Serial);
StvorkaElement myDoor(5,6,A6);
Keypad lockNumpad(makeKeymap(keymap),
                        new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
