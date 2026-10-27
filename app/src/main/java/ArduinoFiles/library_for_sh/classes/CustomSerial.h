#ifndef CUSTOMSERIAL_H
#define CUSTOMSERIAL_H

#include <stdint.h>
#include <Arduino.h>

#define LENGTHBUFFER 16

class CustomSerial {
private:
  // this Hardware Serial to work with
  HardwareSerial* _serial = nullptr;

  uint8_t buffer[2];
  uint8_t dataArray[LENGTHBUFFER];  // buffer dataArray
  uint32_t msBuffer;                // buffer
  void (*packageFunc)(int8_t, uint8_t) = nullptr;
  void (*extraFunc)(int8_t, uint8_t) = nullptr;
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
    uint8_t i = 0;
    boolean loopState = true;
    while (loopState) {
      while (_serial->available() > 0) {
        dataArray[i++] = _serial->read();
        if (i >= length) {
          loopState = false;
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
  }
  /**
    Begin _serial with uint32_t 'baud' rate
  */
  void begin(uint32_t baud) {
    if (_serial!=nullptr) {
      _serial->begin(baud);
    }
  }
  /**
    End _serial
  */
  void end() {
    if (_serial!=nullptr) {
      _serial->end();
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
  void sendPackage(int8_t comma, uint8_t data) {
    if ((_serial!=nullptr)&&(*_serial)) {
      _serial->write(0xFF);
      _serial->write(comma & 0x7F);  // 0x7F = 0b0111 1111
      _serial->write(data);
    }
  }
  /**
    Procedure serializes data to send as Request, sends that dataArray to _serial,
    Max number of functions is 128 bc of signed char
  */
  void sendPackageExtra(int8_t comma, uint8_t length) {
    if ((_serial!=nullptr)&&(*_serial)) {
      _serial->write(0xFF);
      _serial->write(comma | 0x80);  // 0x80 = 0b1000 0000
      _serial->write(adjustToLength(length));
      writeFromDataArray(adjustToLength(length));
    }
  }
  void onPackage(void (*function)(uint8_t, uint8_t)) {
    packageFunc = function;
  }
  void onPackageExtra(void (*function)(uint8_t, uint8_t)) {
    extraFunc = function;
  }
  /**
        Procedure that runs in loop()
    */
  void serialListen() {
    if ((_serial!=nullptr)&&(*_serial)) {
      if (_serial->available() > 0) {
        if (_serial->read() == 0xFF) {
          msBuffer = 0;
          while (msBuffer < 2) {
            if (_serial->available() > 0) {
              buffer[msBuffer++] = _serial->read();
            }
          }
          if (buffer[0] > 0x7F) {
            readToDataArray(buffer[1]);
            if (extraFunc != nullptr) {
              extraFunc(buffer[0] & 0x7F, buffer[1]);
            }
          } else {
            if (packageFunc != nullptr) {
              packageFunc(buffer[0], buffer[1]);
            }
          }
        }
      }
    }
  }
};

#endif