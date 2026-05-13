#include <Arduino.h>
#include <Keypad.h>
#include <stdint.h>

#ifndef LockHandler_h
#define LockHandler_h
#define MAXLENGTHPASSWORD 8

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
  const uint16_t TIMEOUTMS = 10000;
  void (*actionNodes[7])();
  // Writes charDigit in password[8] if countChar < lengthOfPassword, return countChar == lengthOfPassword
  boolean writeCharPass(char charDigit);
  // Procedure that resets password to all 0 & sets countChar at 0
  void resetPassword();
  // Turns values of touchpad button to that boolean value
  void setButtonValue(boolean value);
  // Hash function, returns uint32_t hash value from char* str
  uint32_t hashDJB2(char* str);
  /**
    Procedure that calls when Access granted by LockHandler,
    calls action GRANTACCESS if not nullptr
  */
  void grantAccess();
  /**
    Procedure that complete some action when need to revoke Access to the room
  */
  void revokeAccess();
  /**
    Procedure that complete some action when need to repeat grantAccess() action
  */
  void repeatAccess();
  /**
    Procedure that complete some action when need to do when access denied to the room
  */
  void accessDenied();
public:
  // ARRAY OF POINTERS OF FUNCTIONS TO EXTEND POSSIBLE ACTIONS OF LOCKLISTEN:
  // MAIN ACTIONS:        GRANTACCESS (0), REVOKEACCESS (1),
  // SECONDARY ACTIONS:   REPEATACCESS (2), ACCESSDENIED (3),
  // CONTROL ACTIONS:     REQUESTACCESS(4), TIMEOUT(5), REQUESTABORT(6)
  enum Actions {GRANTACCESS, REVOKEACCESS, REPEATACCESS,
   ACCESSDENIED, REQUESTACCESS, TIMEOUT, REQUESTABORT};
  /** procedure to pass pointer of additional action on type of Actions
    Actions: GRANTACCESS, REVOKEACCESS, REPEATACCESS,
    ACCESSDENIED, REQUESTACCESS, TIMEOUT, REQUESTABORT
  */
  void onAction(void (*function)(), Actions action);
  /** procedure to call passed pointers of function of action on type of Actions
    Actions: GRANTACCESS, REVOKEACCESS, REPEATACCESS,
    ACCESSDENIED, REQUESTACCESS, TIMEOUT, REQUESTABORT
  */
  void callAction(Actions action);
  // Returns boolean stateWritePass
  boolean getStateWritePass();
  /**
    Returns boolean value of locked;
  */
  boolean getLockedState();
  /**
        Constructor with fields
    */
  LockHandler(uint8_t buttonPinIn, uint8_t buttonPinOut, uint8_t* _ledPins, Keypad* keypad,
              char* password, uint8_t length, char _specialSymbol);
  /**
    Returns true if button is true OR keypad got Key pressed, otherwise false
  */
  boolean checkInteraction();
  /**
    Returns true if hash of _password by hashDJB2() == hashPassword 
  */
  boolean checkHashPassword(char* _password);
  /**
    Returns true if hash == hashPassword; otherwise false
  */
  boolean checkHashPassword(uint32_t hash);
  /**
    Return true if this_keypad returns char != NO_KEY, otherwise returns false.
    This char writes to bufferChar
  */
  boolean getBufferChar();
  /**
    Procedure that changes password; max length of password is 8
  */
  void setPassword(char* newPassword, uint8_t length);
  /**
    Procedure that runs in loop(), 
  */
  int8_t lockListen();
};

#endif