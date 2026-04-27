#include <Arduino.h>

Class LedHandler {
private:
    /**
        uint8_t number of posible connetcion Led Lines
    */
    uint8_t numberOfConnetions;
    /**
        uint8_t array of pins, made dinamicly in constructor. Length array corresponds with numberOfConnections
    */
    uint8_t* arrayOfPins;
    /**
        uint8_t array of data which used in OUTPUT on pins, needed to request current brightness of leds
    */
    uint8_t* arrayOfLedData;
public:
    /**
        Default constructor
    */
    LedHandler() {
        numberOfConnections = 0;
        arrayOfPins = nullptr;
        arrayOfLedData = nullptr;
    }
    /**
        Constructor with fields: numberOfConnections, uint8_t ptr (must match numberOfConnections)
    */
    LedHandler(uint8_t newNumberOfPins, uint8_t* newPins) {
        numberOfConnections = newNumberOfPins;
        arrayOfPins = newPins;
        arrayOfLedData = new uint8_t[numberOfConnections];
        for (uint16_t i = 0; i < numberOfConnections; i++) {
            pinMode(arrayOfPins[i], OUTPUT);
            arrayOfLedData[i] = 0xFF;
        }
    }
    /**
        Returns representation of this object in type of uint8_t where first element is numberOfConections,
        length of array = numberOfConnections + 1
    */
    uint8_t* toByteArray() {
        uint8_t* byteArray = new uint8_t[numberOfConnections+1];
        byteArray[0] = numberOfConnections;
        for (uint16_t i = 0; i < numberOfConnections; i++) {
            byteArray[i+1] = arrayOfPins[i]; 
        }
        return byteArray;
    }
    /**
        Function setting pointer of array uint8_t
    */
    void setPins(uint8_t* newPins) {
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
        if (ledId < numberOfConnections) {
            if (Serial) Serial.println("Index out of range!");
            return 1;
        }
        analogWrite(arrayOfPins[ledId], newValue);
        arrayOfLedData[ledId] = newValue;
        return 0;
    }
    uint8_t setValue(uint8_t ledId, uint8_t newValue, uint32_t ms) {
        if (numberOfConnections == 0) {
            if (Serial) Serial.println("NumberOfConnections = 0");
            return 1;
        }
        if (ledId < numberOfConnections) {
            if (Serial) Serial.println("Index out of range!");
            return 1;
        }
        
        int16_t differenceValue = static_cast<int16_t>(newValue) - static_cast<int16_t>(arrayOfLedData[ledId]);
        int16_t absDifference = abs(differenceValue);
        uint8_t value_dx = absDifference / differenceValue;
        uint16_t ms_dx = ms / absDifference;
        for (uint16_t i = 0; i < absDifference; i++) {
            delay(ms_dx);
            arrayOfLedData[ledId] += value_dx;
            analogWrite(arrayOfPins[ledId],arrayOfLedData[ledId]);
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
            arrayOfLedData[i] = newValue;
        }
        return 0;
    }
    /**
        TO DO figure out mathematics behind ms_dx & value_dx
    */
    // uint8_t setValueAll(uint8_t newValue, uint32_t ms) {
    //     if (numberOfConnections == 0) {
    //         if (Serial) Serial.println("NumberOfConnections = 0");
    //         return 1;
    //     }
    //     int16_t differenceValue = static_cast<int16_t>(newValue) - static_cast<int16_t>(arrayOfLedData[ledId]);
    //     int16_t absDifference = abs(differenceValue);
    //     uint8_t value_dx = absDifference / differenceValue;
    //     uint16_t ms_dx = ms / absDifference;
    //     for (uint16_t i = 0; i < absDifference; i++) {
    //         delay(ms_dx);
    //         arrayOfLedData[ledId] += value_dx;
    //         analogWrite(arrayOfPins[ledId],arrayOfLedData[ledId]);
    //     }
    //     return 0;
    // } 
};