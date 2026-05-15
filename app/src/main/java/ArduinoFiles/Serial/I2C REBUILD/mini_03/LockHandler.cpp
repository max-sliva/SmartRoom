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
    if (actionNodes[action] != nullptr) {
      actionNodes[action]();
    }
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
  void LockHandler::revokeAccess() {
    locked = true;
    digitalWrite(ledPins[0],LOW);
    digitalWrite(ledPins[1],HIGH);
    callAction(REVOKEACCESS);
    delay(500);
  }
  void LockHandler::repeatAccess() {
    digitalWrite(ledPins[0],LOW);
    delay(100);
    callAction(REPEATACCESS);
    digitalWrite(ledPins[0],HIGH);
    delay(500);
  }
  void LockHandler::accessDenied() {
    resetPassword();
    digitalWrite(ledPins[1],LOW);
    delay(100);
    digitalWrite(ledPins[1],HIGH);
    callAction(ACCESSDENIED);
    delay(500);
  }
  boolean LockHandler::getStateWritePass() {
    return stateWritePass;
  }
  boolean LockHandler::getLockedState() {
    return locked;
  }
  LockHandler::LockHandler(uint8_t buttonPinIn, uint8_t buttonPinOut, uint8_t* _ledPins, Keypad* keypad,
              char* password, uint8_t length, char _specialSymbol) {
    this_keypad = keypad;
    butPinIn = buttonPinIn;
    butPinOut = buttonPinOut;
    ledPins[0] = _ledPins[0];
    ledPins[1] = _ledPins[1];
    hashPassword = hashDJB2(password);
    lengthOfPassword = length;
    specialSymbol = _specialSymbol;
    for (uint8_t i = 0; i < 7; i++) {
      actionNodes[i] = nullptr;
    }

    pinMode(butPinIn, OUTPUT);
    pinMode(butPinOut, INPUT);
    pinMode(ledPins[0], OUTPUT);
    pinMode(ledPins[1], OUTPUT);

    setButtonValue(LOW);
    revokeAccess();
  }
  boolean LockHandler::checkInteraction() {
    return digitalRead(butPinOut) || getBufferChar();
  }
  boolean LockHandler::checkHashPassword(char* _password) {
    return hashPassword == hashDJB2(_password);
  }
  boolean LockHandler::checkHashPassword(uint32_t hash) {
    return hashPassword == hash;
  }
  boolean LockHandler::getBufferChar() {
    bufferChar = this_keypad->getKey();
    if (bufferChar != NO_KEY) {
      return true;
    }
    return false;
  }
  void LockHandler::setPassword(char* newPassword, uint8_t length) {
    char bufferPassword[MAXLENGTHPASSWORD];
    for (uint8_t i = 0; (i < MAXLENGTHPASSWORD)&&(i < length); i++) {
      bufferPassword[i] = newPassword[i];
    }
    if (length > MAXLENGTHPASSWORD) {
      lengthOfPassword = MAXLENGTHPASSWORD;
    }
    else {
      lengthOfPassword = length;
    }
    hashPassword = hashDJB2(bufferPassword);
    countChar = 0;
    stateWritePass = false;
  }
  int8_t LockHandler::lockListen() {
    if (locked) {
      if (stateWritePass == false) {
        if (checkInteraction() == true) {
          setButtonValue(HIGH);
          if (bufferChar != NO_KEY) {
            writeCharPass(bufferChar);
          }
          stateWritePass = true;
          callAction(REQUESTACCESS);
          timeRequest = millis();
          return 1;
        }
        return 0;
      } else {
        if (digitalRead(butPinOut) == LOW) {
          stateWritePass = false;
          resetPassword();
          callAction(REQUESTABORT);
          return -1;
        } else {
          if (getBufferChar()) {
            timeRequest = millis();
            if (writeCharPass(bufferChar)) {
              digitalWrite(ledPins[0],HIGH);
              delay(100);
              digitalWrite(ledPins[0],LOW);
              if (checkHashPassword(password)) {
                grantAccess();
                return 1;
              }
              else {
                accessDenied();
                return -1;
              }
            }
          }
          if (millis() - timeRequest >= TIMEOUTMS) {
            stateWritePass = false;
            resetPassword();
            setButtonValue(LOW);
            callAction(TIMEOUT);
            return -1;
          }
          return 0;
        }
      }
    } else {
      if (checkInteraction() == true) {
        if (bufferChar == specialSymbol) {
          lockCounter++;
          digitalWrite(ledPins[1],HIGH);
          delay(100);
          digitalWrite(ledPins[1],LOW);
          if (lockCounter >= 3) {
            revokeAccess();
            return -1;
          }
        }
        else {
          lockCounter = 0;
          repeatAccess();
          setButtonValue(LOW);
          return 0;
        }
      }
      return 0;
    }
  }