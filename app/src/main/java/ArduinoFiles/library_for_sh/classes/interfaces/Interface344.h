#ifndef INTERFACE344_H
#define INTERFACE344_H

#ifndef INTERFACE_H
#include "Interface.h"
#endif

#include "../FanElement.h"
// INTERFACE FOR NEXTION -> MEGA FANS
class Interface344 : public Interface {
private:
    FanElement* _array;

    void setValueSingle(uint8_t index, uint8_t value) {
        _array[index].setAccuratePWMValue(value);
    }

    void setValueAll(uint8_t value) {
        for (uint8_t i = 0; i < _length; i++) {
            _array[i].setAccuratePWMValue(value);
        }
    }
public:
    Interface344(uint8_t length) : Interface(344) {
        _length = length;
        _array = new FanElement[length];
    }

    Interface344(uint8_t length, uint8_t screen) : Interface(344,screen) {
        _length = length;
        _array = new FanElement[length];
    }

    ~Interface344() {
        delete[] _array;
    }

    void setElement(uint8_t index,const FanElement& element) {
        _array[index] = element;
    }

    uint8_t getValue(uint8_t index) {
        return _array[index].getAccuratePWMValue();
    }
};

#endif