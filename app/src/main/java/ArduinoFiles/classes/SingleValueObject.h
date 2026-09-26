#include "DeviceObject.h"
#ifndef SingleValueObject_h
#define SingleValueObject_h
/**
    Abstract class that represents a device connecting to SmartHouse system,
    that device has single real value which it work with
*/
class SingleValueObject : public DeviceObject {
protected:
    int value;
public:
    const int MINVALUE = 0;
    const int MAXVALUE = 1023;

    SingleValueObject() : DeviceObject() {}

    SingleValueObject(int minValue, int maxValue) : DeviceObject(), MINVALUE(minValue),  MAXVALUE(maxValue) {}

    SingleValueObject(const unsigned int id) : DeviceObject(id) {}

    SingleValueObject(const unsigned int id, int minValue, int maxValue) : DeviceObject(id), MINVALUE(minValue),  MAXVALUE(maxValue) {}

    virtual void setValue(int v) = 0;

    virtual void setValueOverTime(int v, long ms) = 0;

    virtual int getValue() {
        return this.value;
    }

    virtual int updateValue() {
        return this.value;
    }
};
#endif