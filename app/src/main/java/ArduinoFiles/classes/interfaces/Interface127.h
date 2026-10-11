#ifndef INTERFACE127_H
#define INTERFACE127_H

#ifndef INTERFACE_H
#include "Interface.h"
#endif

#include "../Blinds.h"
// INTERFACE FOR NEXTION -> MEGA BLINDS
class Interface127 : public Interface {
private:
    Blinds* _array;

    void setValueSingle(uint8_t index, uint8_t value) {
        _array[index].moveToValue(value);
    }

    void setValueAll(uint8_t value) {
        for (uint8_t i = 0; i < _length; i++) {
            _array[i].moveToValue(value);
        }
    }
public:
    Interface127(uint8_t length) : Interface(127) {
        _length = length;
        _array = new Blinds[length];
    }

    void setElement(uint8_t index, Blinds* element) {
        _array[index] = element;
    }

    uint8_t getValue(uint8_t index) {
        return _array[index].getCurrentValue();
    }
};

#endif