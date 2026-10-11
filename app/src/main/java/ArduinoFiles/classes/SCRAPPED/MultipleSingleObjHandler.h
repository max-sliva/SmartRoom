#include "DeviceObject.h"
#include "SingleValueObject.h"

#ifndef MultipleSingleObjHandler_h
#define MultipleSingleObjHandler_h

class MultipleSingleObjHandler : public DeviceObject {
private:
    const int numberOfObjects;
    int* values;
    SingleValueObject* objects;

    void setValuesAsync(int value, long ms);

    void setValuesSync(int value, long ms);

public:
    MultipleSingleObjHandler(const unsigned int id, const int nObjects);

    bool allEquals();

    int* updateValues();

    int* getValues();

    void setValue(int v, int index);

    void setValueAll(int v);

    void setValueAllOverTime(int v, long ms);
};

#endif