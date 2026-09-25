#include <math.h>
#include <stdint.h>
#include <Arduino.h>

#ifndef LedHandler_h
#define LedHandler_h
#define MAXNUMBEROFLEDS 8

class LedHandler {
private:
  // uint8_t number of posible connetcion Led Lines
  uint8_t numberOfConnections;
  // uint8_t array of pins, made dinamicly in constructor. Length array corresponds with numberOfConnections
  uint8_t arrayOfPins[MAXNUMBEROFLEDS];
  // uint8_t array of data which used in OUTPUT on pins, needed to request current brightness of leds
  uint8_t arrayOfData[MAXNUMBEROFLEDS];
  // buffers for change led value overtime
  int16_t* bufferArray;
public:
  // Default constructor
  LedHandler() {
    numberOfConnections = 0;
    bufferArray = nullptr;
  }
  // Constructor with fields
  LedHandler(uint8_t newNumberOfPins, uint8_t* newPins) {
    if (newNumberOfPins > MAXNUMBEROFLEDS) {
      numberOfConnections = MAXNUMBEROFLEDS;
    } else {
      numberOfConnections = newNumberOfPins;
    }
    for (uint8_t i = 0; i < numberOfConnections; i++) {
      arrayOfPins[i] = newPins[i];
      pinMode(arrayOfPins[i], OUTPUT);
      setValue(i, 0xFF);
    }
    bufferArray = new uint16_t[numberOfConnections * 2];
  }
  uint8_t getNumberOfConnetcion() {
    return numberOfConnections;
  }
  /**
    Returns value of arrayOfData[ledId] if ledId between 0 or numberOfConnections 
  */
  uint8_t getValue(uint8_t ledId) {
    if ((ledId < 0) || (ledId >= numberOfConnections)) {
      return 0;
    }
    return arrayOfData[ledId];
  }
  /**
        Function setting newValue to Output on pin ledId corresponds in array, return 0 if succesful, 1 otherwise
    */
  uint8_t setValue(uint8_t ledId, uint8_t newValue) {
    if ((ledId < 0) || (ledId >= numberOfConnections)) {
      return 1;
    }
    analogWrite(arrayOfPins[ledId], newValue);
    arrayOfData[ledId] = newValue;
    return 0;
  }
  uint8_t setValue(uint8_t ledId, uint8_t newValue, uint32_t ms) {
    if ((ledId < 0) || (ledId >= numberOfConnections)) {
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
    if (bufferArray == nullptr) {
      return 1;
    }
    if (numberOfConnections == 1) {
      return setValue(0, newValue, ms);
    }
    if (ms == 0) {
      return setValueAll(newValue);
    }
    int16_t buffer, maxLastDelay = 1;
    for (uint8_t i = 0; i < numberOfConnections; i++) {
      buffer = newValue - arrayOfData[i];
      if (buffer < 0) {
        bufferArray[2 * i] = -1;
      } else {
        bufferArray[2 * i] = 1;
      }
      bufferArray[2 * i + 1] = ms / abs(buffer);
      if (maxLastDelay < (ms % bufferArray[2 * i + 1])) maxLastDelay = ms % bufferArray[2 * i + 1];
    }
    for (uint32_t i = 0; i < ms; i++) {
      for (buffer = 0; buffer < numberOfConnections; buffer++) {
        if ((i % bufferArray[2 * buffer + 1]) == 0) {
          if (((arrayOfData[buffer] + bufferArray[2 * buffer]) >= 255) || ((arrayOfData[buffer] + bufferArray[2 * buffer]) < 0)) {
            continue;
          } else {
            arrayOfData[buffer] += bufferArray[2 * buffer];
            analogWrite(arrayOfPins[buffer], arrayOfData[buffer]);
          }
        }
      }
      delay(1);
    }
    delay(maxLastDelay);
    setValueAll(newValue);
    return 0;
  }
};

#endif