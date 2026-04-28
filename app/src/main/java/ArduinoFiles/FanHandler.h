#include <Arduino.h>

class FanHandler {
private:
    uint8_t numberOfConnections;
    uint8_t* arrayOfPins;
    uint8_t* arrayOfData;
    uint8_t minPWMvalue;
public:
    FanHandler() {
        numberOfConections = 0;
        minPWMvalue = 0;
        arrayOfPins = nullptr;
        arrayOfData = nullptr;
    }
    FanHandler(uint8_t newNumberOfPins, uint8_t * newPins, uint8_t _minPWMvalue) {
        numberOfConnections = newNumberOfPins;
        arrayOfPins = newPins;
        arrayOfData = new uint8_t[numberOfConnections];
        for (uint8_t i = 0; i < numberOfConnections; i++) {
            arrayOfData[i] = 0x00;
            pinMode(arrayOfPins[i],OUTPUT);
        }
        minPWMvalue = _minPWMvalue;
    }
    void setPins(uint8_t * newPins) {
    if (arrayOfPins != nullptr) {
      delete[] arrayOfPins;
    }
    arrayOfPins = newPins;
    }
    uint8_t writeNewValueData(uint8_t fanId, uint8_t newValue) {
    if (numberOfConnections == 0) {
        if (Serial) Serial.println("NumberOfConnections = 0");
        return 1;
    }
    if (fanId >= numberOfConnections) {
        if (Serial) Serial.println("Index out of range!");
        return 1;
    }
    arrayOfData[fanId] = getAccurateValue(value);

    }
    uint8_t getAccurateValue(uint8_t value) {
        return map(value,0,255,minPWMvalue,255);
    } 
    uint8_t setValue(uint8_t fanId, uint8_t newValue) {
    if (numberOfConnections == 0) {
      if (Serial) Serial.println("NumberOfConnections = 0");
      return 1;
    }
    if (fanId >= numberOfConnections) {
      if (Serial) Serial.println("Index out of range!");
      return 1;
    }
    uint8_t value = getAccurateValue(newValue);
    analogWrite(arrayOfPins[fanId], getAccurateValue(value));
    writeNewValueData(fanId, new)
    return 0;
  }