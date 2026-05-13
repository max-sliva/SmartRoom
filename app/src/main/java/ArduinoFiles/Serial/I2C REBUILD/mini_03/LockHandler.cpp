#ifndef LockHandler_h
#include "LockHandler.h"
#endif

  boolean LockHandler::writeCharPass(char charDigit) {
    if (countChar < lengthOfPassword) {
      password[countChar++] = charDigit;
    }
    digitalWrite(ledPins[0], HIGH);
    delay(50);
    digitalWrite(ledPins[0], LOW);
    return countChar >= lengthOfPassword;
  }
  void LockHandler::resetPassword() {
    for (uint8_t i = 0; i < MAXLENGTHPASSWORD; i++) {
      password[i] = 0;
    }
    bufferChar = NO_KEY;
    countChar = 0;
    lockCounter = 0;
  }
  void LockHandler::setButtonValue(boolean value) {
    if (digitalRead(butPinOut) != value) {
      digitalWrite(butPinIn, HIGH);
      delay(50);
      digitalWrite(butPinIn, LOW);
    }
  }
  uint32_t LockHandler::hashDJB2(char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash;
  }
  void LockHandler::onAction(void (*function)(), Actions action) {
    actionNodes[action] = function;
  }
  void LockHandler::callAction(Actions action) {
    actionNodes[action]();
  }
  void LockHandler::grantAccess() {
    locked = false;
    stateWritePass = false;
    setButtonValue(LOW);
    resetPassword();
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[0], HIGH);
    callAction(GRANTACCESS);
    delay(500);
  }