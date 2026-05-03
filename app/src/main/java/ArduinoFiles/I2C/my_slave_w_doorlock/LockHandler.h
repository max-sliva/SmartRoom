#include <Arduino.h>
#include <Keymap.h>
#include <stdint.h>

class LockHandler {
private:
    char bufferChar;    // buffer char for this_keypad->getKey()
    char specialSymbol; // special symbol on keymap
    uint8_t ledPins[2]; // pins for leds on lock panel
    // BUTTON PINS
    uint8_t butPinIn, butPinOut;
    // KEYPAD
    Keypad* this_keypad;
    // PASSWORD
    char password[8];   // min 3 (preferable), max 8
    uint32_t hashPassword;  // hash value from hashDJB2(password)
    uint8_t countChar;  // count for filled password characters
    uint8_t lengthOfPassword;
    // BOOLEAN VALUES
    boolean locked;         // boolean value that shows door locked or not
    boolean stateWritePass = false; // boolean value that shows password is currently filling or not
    boolean stateLocking = false;   // boolean value that helps in checking time for closing lock
    // 32 bit long value to check time out when requesting enter
    uint32_t timeRequest;
    uint32_t time_ms;
    uint16_t timeOut = 30000;
    /**
        Writes charDigit in password[8] if countChar < lengthOfPassword, return countChar == lengthOfPassword
    */
    boolean writeCharPass(char charDigit) {
        if (countChar < lengthOfPassword) {
            password[countChar++] = charDigit;
        }
        digitalWrite(ledPins[1],HIGH);
        delay(100);
        digitalWrite(ledPins[1],LOW);
        return countChar >= lengthOfPassword;   
    }
    /**
        Procedure that resets password to all 0 & sets countChar at 0
    */
    void resetPassword() {
        for (uint8_t i = 0; i < 8; i++) {
            password[i] = 0;
        }
        bufferChar = NO_KEY;
        countChar = 0;
    }
    /**
        Turns D-latch on button to boolean value
    */
    void setButtonValue(boolean value) {
        if (digitalRead(butPinOut) != value) {
            digitalWrite(butPinIn,HIGH);
            digitalWrite(butPinIn,LOW);
        }
    }
    /**
        Hash function, returns uint32_t hash value from char* str
    */
    unsigned long hashDJB2(char *str) {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }
    /**
        Procedure that complete some algorith when need to grant enter to room
    */
    void grantEnter() {
        locked = false;
        digitalWrite(ledPins[0],LOW);
        digitalWrite(ledPins[1],HIGH);
        delay(1000);
        digitalWrite(ledPins[0],LOW);
    }
public:
    /**
        Constructor with fields
    */
    LockHandler(uint8_t buttonPinIn, uint8_t buttonPinOut, uint8_t* _ledPins, Keypad* keypad,
         const char[8] password, uint8_t length, char _specialSymbol) {
        this_keypad = keypad;
        butPinIn = buttonPinIn;
        butPinOut = buttonPinOut;
        ledPins = _ledPins;
        hashPassword = hashDJB2(password)
        lengthOfPassword = length;
        SpecialSymbol = _specialSymbol;
        locked = true;
    }
    /**
        Returns true if button is true OR keypad got Key pressed, otherwise false
    */
    boolean checkInteraction() {
        bufferChar = this_keypad->getKey();
        return digitalRead(butPinOut) || (buffer != NO_KEY);
    }
    /**
        Returns true if symbol pressed on keypad or in bufferChar equals symbol, otherwise false
    */
    boolean checkSymbol(char symbol) {
        if (bufferChar == NO_KEY) {
            bufferChar = this_keypad->getKey();
        }
        if (bufferChar == symbol) {
            return true;
        }
        return false;
    }
    /**
        Procedure that changes password
    */
    void changePassword(char[8] newPassword, uint8_t length) {
        lengthOfPassword = length;
        hashPassword = hashDJB2(newPassword);
        countChar = 0;
        stateWritePass = false;
    }
    /**
        Procedure that runs in loop(), 
    */
    int8_t lockListener() {
        if (locked) {
            if (stateWritePass == false) {
                if (checkInteraction() == true) {
                    setButtonValue(HIGH);
                    if (bufferChar != NO_KEY) {
                        writeCharPass(charDigit);
                    }
                    stateWritePass = true;
                    timeRequest = millis();
                    return 1;
                }
                return 0;
            }
            else {
                if (digitalRead(butPinOut) == LOW) {
                    stateWritePass = false;
                    resetPassword();
                    return -1;
                }
                else {
                    bufferChar = this_keypad->getKey();
                    if (bufferChar != NO_KEY) {
                        timeRequest = millis();
                        if (writeCharPass(bufferChar)) {
                            if (hashPassword == hashDJB2(password)) {
                                grantEnter();
                                return 1;
                            }
                        }
                    }   
                    if (millis() - timeRequest >= timeOut) {
                        stateWritePass = false;
                        resetPassword();
                        setButtonValue(LOW);
                        return -1;
                    }
                    return 0;
                }
            }
        }
        else {
            if (checkInteraction() == true) {
                if (checkSymbol(specialSymbol)) {
                    if (stateLocking == false) {
                        time_ms = millis();
                        stateLocking = true;
                    }
                    else {
                        if (millis() - time_ms >= 3000) {
                            locked = true;
                            stateLocking = false;
                            digitalWrite(ledPins[0],HIGH);
                            delay(1000);
                            return 1;
                        }
                    }
                }
                else {
                    stateLocking = false;
                    digitalWrite(ledPins[1],HIGH);
                    delay(1000);
                    digitalWrite(ledPins[0],LOW);
                    setButtonValue(LOW);
                }
            }
            return 0; 
        }
    }
};
