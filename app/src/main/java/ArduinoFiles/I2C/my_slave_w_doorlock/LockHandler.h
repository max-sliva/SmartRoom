#include <Arduino.h>
#include <Keymap.h>
#include <stdint.h>

class LockHandler {
private:
    char bufferChar;
    uint8_t numRows;
    uint8_t numCols;

    uint8_t ledPins[2];

    char* keymap;
    uint8_t* rowPins;
    uint8_t* colPins;
    // BUTTON PINS
    uint8_t butPinIn;
    uint8_t butPinOut;
    // KEYPAD POINTER
    Keypad this_keypad;
    // PASSWORD
    char password[8];
    uint32_t hashPassword;
    uint8_t countChar = 0;
    uint8_t lengthOfPassword;
    boolean locked;

    boolean stateWritePass;
    // 32 bit long value to check time out when requesting enter
    uint32_t timeRequest;
    uint32_t time_ms;
    /**
        Writes charDigit in password[8], return if countChar == lengthOfPassword
    */
    boolean writeCharPass(char charDigit) {
        password[countChar++] = charDigit;
        digitalWrite(ledPins[1],HIGH);
        delay(100);
        digitalWrite(ledPins[1],LOW);
        return countChar == lengthOfPassword;   
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
        Turns D-latch on button to that boolean value
    */
    void setButtonValue(boolean value) {
        if (digitalRead(butPinOut) != value) {
            digitalWrite(butPinIn,HIGH);
            digitalWrite(butPinIn,LOW);
        }
    }
    unsigned long hashDJB2(char *str) {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }
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

    boolean checkInteraction() {
        bufferChar = this_keypad.getKey();
        return digitalRead(butPinOut) || (buffer != NO_KEY);
    }

    int8_t LockListener() {
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
                    if (millis() - timeRequest >= 60000) {
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
        // if (checkInteraction() == HIGH) {
        //     setButtonValue(HIGH);
        //     if (bufferChar != NO_KEY) {
        //         password[0] = bufferChar;
        //         stateWritePass = true;
        //         countChar++;
        //     }
        //     if (millis() - time_ms > 1000) {
        //         time_ms = millis();
        //         digitalWrite(ledPins[1],HIGH);
        //     } 
        //     if (millis() - time_ms > 200) {
        //         time_ms = millis();
        //         digitalWrite(ledPins[1],LOW);
        //     }
        // }
    }
};
