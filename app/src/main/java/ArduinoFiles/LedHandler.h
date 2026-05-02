#include <stdint.h>
#include <Arduino.h>

class LedHandler {
private:
  /**
        uint8_t number of posible connetcions Led Lines
    */
  uint8_t numberOfConnections;
  /**
        uint8_t array of pins, made dinamicly in constructor. Length array corresponds with numberOfConnections
    */
  uint8_t* arrayOfPins;
  /**
        uint8_t array of data which used in OUTPUT on pins, needed to request current brightness of leds
    */
  uint8_t* arrayOfData;
public:
  /**
        Default constructor
    */
  LedHandler() {
    numberOfConnections = 0;
    arrayOfPins = nullptr;
    arrayOfData = nullptr;
  }
  /**
        Constructor with fields: numberOfConnections, uint8_t ptr (must match numberOfConnections)
    */
  LedHandler(uint8_t newNumberOfPins, uint8_t * newPins) {
    numberOfConnections = newNumberOfPins;
    arrayOfPins = newPins;
    arrayOfData = new uint8_t[numberOfConnections];
    for (uint16_t i = 0; i < numberOfConnections; i++) {
      pinMode(arrayOfPins[i], OUTPUT);
      setValue(i, 0xFF);
    }
  }
  /**
        Returns representation of this object in type of uint8_t where first element is numberOfConections,
        length of array = numberOfConnections + 1
    */
  uint8_t* toByteArray() {
    uint8_t* byteArray = new uint8_t[numberOfConnections + 1];
    byteArray[0] = numberOfConnections;
    for (uint16_t i = 0; i < numberOfConnections; i++) {
      byteArray[i + 1] = arrayOfPins[i];
    }
    return byteArray;
  }
  /**
        Function setting pointer of array uint8_t
    */
  void setPins(uint8_t * newPins) {
    if (arrayOfPins != nullptr) {
      delete[] arrayOfPins;
    }
    arrayOfPins = newPins;
  }
  /**
        Function setting newValue to Output on pin ledId corresponds in array, return 0 if succesful, 1 otherwise
    */
  uint8_t setValue(uint8_t ledId, uint8_t newValue) {
    if (numberOfConnections == 0) {
      if (Serial) Serial.println("NumberOfConnections = 0");
      return 1;
    }
    if (ledId >= numberOfConnections) {
      if (Serial) Serial.println("Index out of range!");
      return 1;
    }
    analogWrite(arrayOfPins[ledId], newValue);
    arrayOfData[ledId] = newValue;
    return 0;
  }
  uint8_t setValue(uint8_t ledId, uint8_t newValue, uint32_t ms) {
    if (numberOfConnections == 0) {
      if (Serial) Serial.println("NumberOfConnections = 0");
      return 1;
    }
    if (ledId >= numberOfConnections) {
      if (Serial) Serial.println("Index out of range!");
      return 1;
    }

    int16_t differenceValue = static_cast<int16_t>(newValue) - static_cast<int16_t>(arrayOfData[ledId]);
    int16_t absDifference = abs(differenceValue);
    uint8_t value_dx = absDifference / differenceValue;
    uint16_t ms_dx = ms / absDifference;
    for (uint16_t i = 0; i < absDifference; i++) {
      delay(ms_dx);
      arrayOfData[ledId] += value_dx;
      analogWrite(arrayOfPins[ledId], arrayOfData[ledId]);
    }
    return 0;
  }
  /**
        Function setting newValue to Output on ALL pin ledId corresponds in array, return 0 if succesful, 1 otherwise
    */
  uint8_t setValueAll(uint8_t newValue) {
    if (numberOfConnections == 0) {
      if (Serial) Serial.println("NumberOfConnections = 0");
      return 1;
    }
    for (uint16_t i = 0; i < numberOfConnections; i++) {
      analogWrite(arrayOfPins[i], newValue);
      arrayOfData[i] = newValue;
    }
    return 0;
  }
  /**
        TO DO figure out mathematics behind ms_dx & value_dx
    */
  uint8_t setValueAll(uint8_t newValue, uint32_t ms) {
      if (numberOfConnections == 0) {
          if (Serial) Serial.println("NumberOfConnections = 0");
          return 1;
      }
      int16_t* value_diff = new int16_t[numberOfConnections];
      int8_t* value_dx = new int8_t[numberOfConnections];
      uint16_t* mod_dx = new uint16_t[numberOfConnections];
      //uint32_t new_ms = ms / 10;
      for (uint8_t i = 0; i < numberOfConnections; i++) {
        value_dx[i] = 1;
        value_diff[i] = newValue - arrayOfData[i];
        if (value_diff[i] < 0) {
          value_dx[i] *= -1;
        }
        mod_dx[i] = round(ms / abs(value_diff[i]));
        Serial.print("value_diff: ");
        Serial.print(value_diff[i]);
        Serial.print("\tvalue_dx: ");
        Serial.print(value_dx[i]);
        Serial.print("\tmod_dx: ");
        Serial.println(mod_dx[i]);
      }
      uint8_t j;
      for (uint32_t i = 0; i < ms; i++) {
        for (j = 0; j < numberOfConnections; j++) {
          if ((i % mod_dx[j]) == 0) {
            if (((arrayOfData[j] + value_dx[j]) < 255) && ((arrayOfData[j] + value_dx[j]) >= 0)) {
              arrayOfData[j] += value_dx[j];
              analogWrite(arrayOfPins[j], arrayOfData[j]);
            }
          }
        }
        delay(1);
      }
      setValueAll(newValue);
      delete value_diff;
      delete value_dx;
      delete mod_dx;
      return 0;
  }
};