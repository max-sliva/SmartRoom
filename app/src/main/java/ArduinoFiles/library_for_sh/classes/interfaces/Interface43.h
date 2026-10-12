#ifndef INTERFACE43_H
#define INTERFACE43_H

#ifndef INTERFACE_H
#include "Interface.h"
#endif

#include "../CustomSerial.h"
// INTERFACE FOR NEXTION -> MEGA -> MINI LEDS
class Interface43 : public Interface {
private:
    uint8_t* _array;
    CustomSerial* _serial = nullptr;

    void setValueSingle(uint8_t index, uint8_t value) {
        _array[index] = value;
        if (_serial != nullptr) {
            _serial->setDataElem(index, 0);
            _serial->setDataElem(value, 1);
            _serial->sendPackageExtra(1, 2);
        }
    }

    void setValueAll(uint8_t value) {
        for (uint8_t i = 0; i < _length; i++) {
            _array[i] = value;
        }
        if (_serial != nullptr) {
            _serial->sendPackage(0,value);
        }
    }
public:
    Interface43(uint8_t length) : Interface(43) {
        _length = length;
        _array = new uint8_t[length];
    }

    void setSerial(CustomSerial* serial) {
        _serial = serial;
    }

    uint8_t getValue(uint8_t index) {
        return _array[index];
    }
};

#endif