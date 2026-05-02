#include "Arduino.h"
/**
    Abstract class for Electronic Elements
*/
class Element {
protected:
    /**
        array* of pins which which this element needed to be used in its tasks
    */
    uint8_t* PINS;
    /**
        uint8_t value of Element which this element needed to be used in its tasks
    */ 
    uint8_t value;
    /**
        Private procedure that sets all power pins in uint8_t* PINS
    */
    void setPins(uint8_t* pins) {
        PINS = pins;
    }
    /**
        Private procedure that sets all pinMode to pins 
    */
    virtual void setPinModes() = 0;
public:
    ~Element() {
        delete[] PINS;
    }
    /**
        Returns uint8_t value
    */
    uint8_t getValue() {
        return value;
    }
    /**
        Returns a pointer to uint8_t array representation of this object
    */
    virtual uint8_t* toByteArray() = 0;
};