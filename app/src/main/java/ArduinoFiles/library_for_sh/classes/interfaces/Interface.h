#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

class Interface {
private:
    const uint16_t _id;
    bool equalsFlag = true;
    const uint8_t _screen;
protected:
    uint8_t _length;

    virtual void setValueSingle(uint8_t index, uint8_t value) = 0;

    virtual void setValueAll(uint8_t value) = 0;
public:
    Interface() : _id(65535), _screen(255) {}

    Interface(const uint16_t id) : _id(id), _screen(255) {}

    Interface(const uint16_t id, const uint8_t screen) : _id(id), _screen(screen) {}

    uint16_t getId() { 
        return _id;
    }

    uint8_t getScreen() {
        return _screen;
    }

    virtual uint8_t length() {
        return _length;
    }

    bool isEquals() {
        return equalsFlag;
    }

    virtual void setValue(uint8_t index, uint8_t value) {
        if (index == 255) {
            setValueAll(value);
            equalsFlag = true;
        } else {
            setValueSingle(index, value);
            equalsFlag = false;
        }
    }

    virtual uint8_t getValue(uint8_t index) = 0;

};

#endif INTERFACE_H