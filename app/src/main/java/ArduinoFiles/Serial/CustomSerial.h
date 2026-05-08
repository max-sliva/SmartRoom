#include <Arduino.h>

class CustomSerial {
private:
    HardwareSerial* thisSerial;
    /**
        Procedure that sends data array of length to uart thisSerial
    */
    void sendData(uint8_t* data, uint8_t length) {
        for (uint8_t i = 0; i < length; i++) {
            thisSerial->write(data[i]);
        }
    }
public:
    /**
        Constructor with field
    */
    CustomSerial(HardwareSerial* serial) {
        thisSerial = serial;
    }   
    /**
        Function that classifies data to send as receive to uart thisSerial
    */
    void sendFunction(uint8_t comma, uint8_t data, uint8_t length) {
        uint8_t dataArray[length+3];
        dataArray[0] = 0x01;
        dataArray[1] = comma;
        dataArray[2] = length;
        for (uint8_t i = 0; i < length; i++) {
            dataArray[i+3] = data[i];
        }
        sendData(dataArray, length+3);
    }
    /**
        Function that classifies data to send as request to uart thisSerial,
        after on serialListener arrives data of "length"
    */
    void sendRequest(uint8_t type, uint8_t length) {
        uint8_t dataArray[3] = { 0x00, type, length};
        sendData(dataArray, 3);
    }
    /**
        Procedure that runs in loop()
    */
    void serialListener() {

    }
};