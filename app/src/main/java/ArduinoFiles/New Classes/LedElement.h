#include "Element.h"

class LedElement : public Element {
private:
    /**
        Private procedure that sets all pinMode to pins 
    */
    void setPinModes() {
        pinMode(*PINS, OUTPUT);
    }
public:
    LedElement() {
        setPins(new int {0});
        value = 0;
    }
    /**
        Constructor with fields
    */
    LedElement(uint8_t _pin) {
        setPins(&_pin);
        setPinModes();
        setValue(0);
    }
    /**
        Constructor by array, used together with readBytesFromEEPROM
    */
    LedElement(uint8_t* array) {
        setPins(array);
        setPinModes();
        setValue(0);
    }
    /**
        Set value to newValue and also turn this LedElement to newValue
    */
    void setValue(uint8_t newValue) {
        analogWrite(*PINS, newValue);
        value = newValue;
    }
    /**
        Returns a pointer to uint8_t array representation of this object
    */
    uint8_t* toByteArray() {
        return PINS;
    }
};