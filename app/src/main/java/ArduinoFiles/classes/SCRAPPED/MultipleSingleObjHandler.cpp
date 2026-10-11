#ifndef MultipleSingleObjHandler_h
#include "MultipleSingleObjHandler.h"
#endif

    MultipleSingleObjHandler::MultipleSingleObjHandler(const unsigned int id, const int nObjects) 
    : DeviceObject(id), numberOfObjects(nObjects) {
        this.values = new int[nObjects];
        this.objects = new SingleValueObject[nObjects];
    }
    /// TODO LATER
    void MultipleSingleObjHandler::setValuesAsync(int value, long ms) {

    }
    /// TODO LATER
    void MultipleSingleObjHandler::setValuesSync(int value, long ms) {

    }

    bool MultipleSingleObjHandler::allEquals() {
        int buffer = objects[0].getValue();
        for (int i = 1; i < numberOfObjects; i++) {
            if (objects[i].getValue() != buffer) return false;
        }
        else return true;
    }

    int* MultipleSingleObjHandler::updateValues() {
        for (int i = 0; i < numberOfObjects; i++) {
            values[i] = objects[i].updateValue();
        }
        return values;
    }

    int* MultipleSingleObjHandler::getValues() {
        return values;
    }

    void MultipleSingleObjHandler::setValue(int v, int index) {
        if ((index < 0) || (index >= numberOfObjects)) {
            return;
        } 
        objects[index].setValue(v);
    }

    void MultipleSingleObjHandler::setValueAll(int v) {
        for (int i = 0; i < numberOfObjects; i++) {
            objects[i].setValue(v);
        }
    }

    void MultipleSingleObjHandler::setValueAllOverTime(int v, long ms) {
        if (allEquals()) {
            setValuesSync(v,ms);
        } else {
            setValuesAsync(v,ms);
        }
    }

    