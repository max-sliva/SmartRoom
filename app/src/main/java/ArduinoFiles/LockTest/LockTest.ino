#include "LockHandler.h"

char keymap[3][4] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' }
};

LockHandler* myLocker;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial started!");
  // KEYPAD
  Keypad myKeypad = Keypad(makeKeymap(keymap),
                           new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);
  char password[8] = { '1', '2', '3', '4', 0, 0, 0, 0 };
  // LOCKHANDLER
  myLocker = new LockHandler(13, A0, new uint8_t[2]{ A4, A5 },
                             &myKeypad, password, 4, keymap[2][3]);
}

void loop() {
  myLocker->lockListener();
}
