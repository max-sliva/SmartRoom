#include <stdint.h>
#include <Arduino.h>

#ifndef CustomSerial_h
#define CustomSerial_h
#define LENGTHBUFFER 32

class CustomSerial {
private:
  // this Hardware Serial to work with
  HardwareSerial* _serial;

  uint8_t buffer[2];
  uint8_t dataArray[LENGTHBUFFER];  // buffer dataArray
  uint32_t ms;                      // buffer
  void (*receiveFunc)(int8_t, uint8_t) = nullptr;
  void (*requestFunc)(int8_t, uint8_t) = nullptr;
  /**
    Procedure that sends number of elements of dataArray by length to _serial
  */
  void writeFromDataArray(uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
      _serial->write(dataArray[i]);
    }
  }
  /**
    Procedure that reads from _serial to dataArray by length
  */
  void readToDataArray(uint8_t length) {
    if (_serial->available() > 0) {
      ms = millis();
      uint8_t i = 0;
      while (i < length) {
        if (_serial->available() > 0) {
          ms = millis();
          dataArray[i++] = _serial->read();
        }
        if ((millis() - ms) >= 5) {
          break;
        }
      }
    }
  }
public:
  /**
        Constructor with field
    */
  CustomSerial(HardwareSerial* serial) {
    _serial = serial;

    while (_serial->available() > 0) {
      _serial->read(); 
    }
  }
  // PROCEDURES TO WORK WITH DATAARRAY
  /**
    Returns element of dataArray on 'index', index will be adjusted to length
  */
  uint8_t getDataElem(uint8_t index) {
    return dataArray[adjustToLength(index)];
  }
  /**
    Sets element of dataArray on 'index' to 'data', index will be adjusted to length
  */
  void setDataElem(uint8_t data, uint8_t index) {
    dataArray[adjustToLength(index)] = data;
  }
  /**
    Resets dataArray to all 0
  */
  void resetDataArray() {
    for (uint8_t i = 0; i < LENGTHBUFFER; i++) {
      dataArray[i] = 0;
    }
  }
  /**
    Returns length adjusted by max value of LENGTHBUFER
  */
  uint8_t adjustToLength(uint8_t length) {
    if (length > LENGTHBUFFER) {
      return LENGTHBUFFER;
    } else {
      return length;
    }
  }
  /**
    Procedure serializes data to send as Recieve, sends that dataArray to _serial
    Max number of functions 128 bc of signed char
  */
  void sendReceive(int8_t comma, uint8_t length) {
    _serial->write(0xFF);
    _serial->write(comma & 0x7F);  // 0x7F = 0b0111 1111
    _serial->write(adjustToLength(length));
    writeFromDataArray(adjustToLength(length));
  }
  /**
    Procedure serializes data to send as Request, sends that dataArray to _serial,
    Max number of functions is 128 bc of signed char
  */
  void sendRequest(int8_t comma, uint8_t length) {
    _serial->write(0xFF);
    _serial->write(comma | 0x80);  // 0x80 = 0b1000 0000
    _serial->write(adjustToLength(length));
  }
  void onReceive(void (*function)(uint8_t, uint8_t)) {
    receiveFunc = function;
  }
  void onRequest(void (*function)(uint8_t, uint8_t)) {
    requestFunc = function;
  }
  /**
        Procedure that runs in loop()
    */
  void serialListener() {
    if (_serial->available() > 0) {
      if (_serial->read() == 0xFF) {
        ms = 0;
        while (ms < 2) {
          if (_serial->available() > 0) {
            buffer[ms++] = _serial->read();
          }
        }
        if (buffer[0] > 0x7F) {
          if (requestFunc != nullptr) {
            requestFunc(buffer[0] & 0x7F, buffer[1]);
          }
        } else {
          if (receiveFunc != nullptr) {
            readToDataArray(buffer[1]);
            receiveFunc(buffer[0], buffer[1]);
          }
        }
      }
    }
  }
};

#endif