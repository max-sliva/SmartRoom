#include <Arduino.h>
#include <Keypad.h>
#include <stdint.h>

#ifndef LockHandler_h
#define LockHandler_h
#define MAXLENGTHPASSWORD 8

#ifndef actionProcedures_h
#include "actionProcedures.h"
#endif

class LockHandler {
private:
  char bufferChar;     // buffer char for this_keypad->getKey()
  char specialSymbol;  // special symbol on keymap
  uint8_t ledPins[2];  // pins for leds on lock panel
  // BUTTON PINS
  uint8_t butPinIn, butPinOut;
  // KEYPAD
  Keypad* this_keypad;
  // PASSWORD
  char password[MAXLENGTHPASSWORD];       // min 3 (preferable), max 8
  uint32_t hashPassword;  // hash value from hashDJB2(password)
  uint8_t countChar = 0;      // count for filled password characters
  uint8_t lengthOfPassword;
  uint8_t lockCounter = 0;
  // BOOLEAN VALUES
  boolean locked;                  // boolean value that shows door locked or not
  boolean stateWritePass = false;  // boolean value that shows password is currently filling or not
  // 32 bit long value to check time out when requesting enter
  uint32_t timeRequest;
  const uint16_t TIMEOUT = 10000;
  /**
        Writes charDigit in password[8] if countChar < lengthOfPassword, return countChar == lengthOfPassword
    */
  boolean writeCharPass(char charDigit) {
    if (countChar < lengthOfPassword) {
      password[countChar++] = charDigit;
    }
    digitalWrite(ledPins[0], HIGH);
    delay(50);
    digitalWrite(ledPins[0], LOW);
    return countChar >= lengthOfPassword;
  }
  /**
        Procedure that resets password to all 0 & sets countChar at 0
    */
  void resetPassword() {
    for (uint8_t i = 0; i < MAXLENGTHPASSWORD; i++) {
      password[i] = 0;
    }
    bufferChar = NO_KEY;
    countChar = 0;
    lockCounter = 0;
  }
  /**
        Turns D-latch on button to boolean value
    */
  void setButtonValue(boolean value) {
    if (digitalRead(butPinOut) != value) {
      digitalWrite(butPinIn, HIGH);
      delay(50);
      digitalWrite(butPinIn, LOW);
    }
  }
  /**
        Hash function, returns uint32_t hash value from char* str
    */
  uint32_t hashDJB2(char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
      hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash;
  }
  /**
        Procedure that complete some action when need to grant Access to the room
    */
  void grantAccess() {
    locked = false;
    stateWritePass = false;
    setButtonValue(LOW);
    resetPassword();
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[0], HIGH);
    grantAccessAction();
    delay(500);
  }
  /**
    Procedure that complete some action when need to revoke Access to the room
  */
  void revokeAccess() {
    locked = true;
    digitalWrite(ledPins[0],LOW);
    digitalWrite(ledPins[1],HIGH);
    //revokeAccessAction();
    delay(500);
  }
  /**
    Procedure that complete some action when need to repeat grantAccess() action
  */
  void repeatAccess() {
    digitalWrite(ledPins[0],LOW);
    delay(100);
    repeatAccessAction();
    digitalWrite(ledPins[0],HIGH);
    delay(500);
  }

  void accessDenied() {
    resetPassword();
    digitalWrite(ledPins[1],LOW);
    delay(100);
    digitalWrite(ledPins[1],HIGH);
    accessDeniedAction();
    delay(500);
  }
public:
  uint8_t getLockedState() {
    return locked;
  }
  /**
        Constructor with fields
    */
  LockHandler(uint8_t buttonPinIn, uint8_t buttonPinOut, uint8_t* _ledPins, Keypad* keypad,
              char* password, uint8_t length, char _specialSymbol) {
    this_keypad = keypad;
    butPinIn = buttonPinIn;
    butPinOut = buttonPinOut;
    ledPins[0] = _ledPins[0];
    ledPins[1] = _ledPins[1];
    hashPassword = hashDJB2(password);
    lengthOfPassword = length;
    specialSymbol = _specialSymbol;
    pinMode(butPinIn, OUTPUT);
    pinMode(butPinOut, INPUT);
    pinMode(ledPins[0], OUTPUT);
    pinMode(ledPins[1], OUTPUT);

    setButtonValue(LOW);
    revokeAccess();
  }
  /**
    Returns true if button is true OR keypad got Key pressed, otherwise false
  */
  boolean checkInteraction() {
    return digitalRead(butPinOut) || getBufferChar();
  }
  /**
    Returns true if hash of _password by hashDJB2() == hashPassword 
  */
  boolean checkHashPassword(char* _password) {
    return hashPassword == hashDJB2(_password);
  }
  /**
    Returns true if hash == hashPassword; otherwise false
  */
  boolean checkHashPassword(uint32_t hash) {
    return hashPassword == hash;
  }
  /**
    Return true if this_keypad returns char != NO_KEY, otherwise returns false.
    This char writes to bufferChar
  */
  boolean getBufferChar() {
    bufferChar = this_keypad->getKey();
    if (bufferChar != NO_KEY) {
      return true;
    }
    return false;
  }
  /**
    Procedure that changes password; max length of password is 8
  */
  void setPassword(char* newPassword, uint8_t length) {
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
  /**
    Procedure that runs in loop(), 
  */
  int8_t lockListen() {
    if (locked) {
      if (stateWritePass == false) {
        if (checkInteraction() == true) {
          setButtonValue(HIGH);
          if (bufferChar != NO_KEY) {
            writeCharPass(bufferChar);
          }
          stateWritePass = true;
          Serial.println("Enter");
          //requestAccessAction();
          timeRequest = millis();
          return 1;
        }
        return 0;
      } else {
        if (digitalRead(butPinOut) == LOW) {
          stateWritePass = false;
          resetPassword();
          requestAbortAction();
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
          if (millis() - timeRequest >= TIMEOUT) {
            stateWritePass = false;
            resetPassword();
            setButtonValue(LOW);
            timeOutAction();
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
};

#endif