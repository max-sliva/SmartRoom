#include <Arduino.h>
#include <Keymap.h>
#include <stdint.h>

class LockHandler {
private:
    char bufferChar;    // buffer char for this_keypad.getKey()
    uint8_t ledPins[2]; // pins for leds on lock panel
    // VALUES FOR KEYPAD
    uint8_t numRows, numCols;
    // POINTER FOR ARRAYS OF PINS & KEYMAP
    char* keymap;
    uint8_t* rowPins, colPins;
    // BUTTON PINS
    uint8_t butPinIn, butPinOut;
    // KEYPAD
    Keypad this_keypad;
    // PASSWORD
    char password[8];   // min 3, max 8
    uint32_t hashPassword;  // hash value from hashDJB2(password)
    uint8_t countChar = 0;  // count for filled password characters
    uint8_t lengthOfPassword;
    // BOOLEAN VALUES
    boolean locked; // boolean value that shows door locked or not
    boolean stateWritePass; // boolean value that shows password is currently filling or not
    // 32 bit long value to check time out when requesting enter
    uint32_t timeRequest;
    uint32_t time_ms;
    uint16_t timeOut;
    /**
        Writes charDigit in password[8], return if countChar == lengthOfPassword
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
    LockHandler() {

    }
    /**
        Returns true if button is true OR keypad got Key pressed
    */
    boolean checkInteraction() {
        bufferChar = this_keypad.getKey();
        return digitalRead(butPinOut) || (buffer != NO_KEY);
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
                    bufferChar = this_keypad.getKey();
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
                digitalWrite(ledPins[1],HIGH);
                delay(1000);
                digitalWrite(ledPins[0],LOW);
                setButtonValue(LOW);
            }
            return 0; 
        }
    }
};
