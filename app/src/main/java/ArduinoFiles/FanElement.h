#include <Arduino.h>

class FanElement{
private:
    uint8_t pin;
    uint8_t value;
    uint8_t minPWMvalue;
public:
    FanElement() {
        pin = 0;
        value = 0;
        minPWMvalue = 0;
    }
    FanElement(uint8_t _pin, uint8_t _minPWMvalue) {
        pin = _pin;
        minPWMvalue = _minPWMvalue;
        pinMode(pin,OUTPUT);
    }
    uint8_t getValue() {
        return value;
    }
    uint8_t getMinPWMValue() {
        return minPWMvalue;
    }
    uint8_t getAccuratePWMValue(uint8_t rawValue) {
        if (rawValue <= minPWMvalue) {
            return 0x00;
        }
        return map(rawValue,0,255,minPWMvalue,255);
    }
    void setAccuratePWMValue(uint8_t rawValue) {
        value = getAccuratePWMValue(rawValue);
        analogWrite(pin,value);
    }
};