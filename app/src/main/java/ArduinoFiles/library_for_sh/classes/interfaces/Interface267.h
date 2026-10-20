#ifndef INTERFACE267_H
#define INTERFACE267_H

#ifndef INTERFACE_H
#include "Interface.h"
#endif

#include "../StvorkaElement.h"
// INTERFACE FOR NEXTION -> MEGA STVORKAS
class Interface267 : public Interface {
private:
    StvorkaElement* _array;

    void setValueSingle(uint8_t index, uint8_t value) {
        _array[index].moveRelativelyToByte(value);
    }

    void setValueAll(uint8_t value) {
        for (uint8_t i = 0; i < _length; i++) {
            _array[i].moveRelativelyToByte(value);
        }
    }
public:
    Interface267(uint8_t length) : Interface(267) {
        _length = length;
        _array = new StvorkaElement[length];
    }

    void setElement(uint8_t index, const StvorkaElement& element) {
        _array[index] = element;
    }

    uint8_t getValue(uint8_t index) {
        return _array[index].getCurrentValue();
    }
};

#endif