#include <stdint.h>
#include <Arduino.h>

#ifndef CustomSerial_h
#define CustomSerial_h
#define LENGTHBUFFER 16

class CustomSerial {
private:
  // this Hardware Serial to work with
  HardwareSerial* thisSerial;
  // state that tells that object stopping listening for sending data and beginnning recieve data
  uint32_t time_ms;
  // buffer ArrayData,
  // exist to exclude a lot of memory alloc for new ArrayData for every single procedure
  uint8_t paramsArray[3];
  uint8_t dataArray[LENGTHBUFFER];
  void  (*onReceiveFunction) ();
  /**
    Procedure that sends number of elements of dataArray by length to thisSerial
  */
  void sendDataArray(uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
      thisSerial->write(dataArray[i]);
    } 
  }
  /**
    Procedure that specifically sends 3 bytes that reads like instructions
  */
  void sendParamsArray() {
    if (paramsArray[0] < 0x03) {
      for (uint8_t i = 0; i < 3; i++) {
        thisSerial->write(paramsArray[i]);
      }
      paramsArray[0] = 0xFF;
    }
  }
public:
  /**
        Constructor with field
    */
  CustomSerial(HardwareSerial* serial) {
    thisSerial = serial;
    paramsArray[0] = 0xFF;
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
    Returns one of the three parametrs, if index > 3 returns 0
  */
  uint8_t getParam(uint8_t index) {
    if (index > 3) {
      return 0;
    }
    return paramsArray[index];
  }
  /**
    Resets arrayData to all-0 array
  */
  void resetArrayData() {
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
    }
    else {
      return length;
    }
  }
  /**
    Procedure serializes data to send as Recieve, sends that dataArray to thisSerial
  */
  void sendReceive (uint8_t comma, uint8_t length) {
    paramsArray[0] = 0x00;
    paramsArray[1] = comma;
    paramsArray[2] = adjustToLength(length);
    sendParamsArray();
    sendDataArray(paramsArray[2]);
  }
  /**
    Procedure serializes data to send as Request, sends that dataArray to thisSerial
  */
  void sendRequest(uint8_t type, uint8_t length) {
    paramsArray[0] = 0x01;
    paramsArray[1] = type;
    paramsArray[2] = adjustToLength(length);
    sendParamsArray();
  }
  /**
    Procedure serializes data that sends on Request, sends that dataArray to thisSerial
  */
  void sendDataOnRequest(uint8_t type, uint8_t length) {
    paramsArray[0] = 0x02;
    paramsArray[1] = type;
    paramsArray[2] = adjustToLength(length);
    sendParamsArray();
    sendDataArray(paramsArray[2]);
  }
  void onReceive(void (*function) ()) {
    onReceiveFunction = function;
  }
  /**
        Procedure that runs in loop()
    */
  void serialListener() {
    if (thisSerial->available() > 3) {
      for (uint8_t i = 0; i < 3; i++) {
        paramsArray[i] = thisSerial->read();
      }
      if (paramsArray < 0x03) {
        onReceiveFunction();
      } 
    }
  }
};

#endif