#include <stdint.h>
#include <Arduino.h>

class CustomSerial {
private:
  // this Hardware Serial to work with
  HardwareSerial* thisSerial;
  // state that tells that object stopping listening for sending data and beginnning recieve data
  boolean recieving;
  uint32_t time_ms;
  uint8_t dataType;
  // buffer ArrayData,
  // exist to exclude a lot of memory alloc for new ArrayData for every single procedure
  const uint8_t LENGTHBUFFERARRAY = 32;
  uint8_t arrayData[LENGTHBUFFERARRAY];
  /**
    Procedure that sends data array of length to uart thisSerial
  */
  void sendData(uint8_t length) {
    if (length > LENGTHBUFFERARRAY) {
      length = LENGTHBUFFERARRAY;
    }
    for (uint8_t i = 0; i < length; i++) {
      thisSerial->write(dataArray[i]);
    }
    resetArrayData();
  }
  /**
    Procedure that recieve some amount of data of length
  */
  boolean recieveData(uint8_t length) {
    boolean state = false;
    if (length > LENGTHBUFFERARRAY) {
      length = LENGTHBUFFERARRAY;
    }
    uint8_t counter = 0;
    time_ms = millis();
    while (counter < length) {
      if (thisSerial.isAvailable() > 0) {
        dataArray[counter++] = thisSerial.read();
        time_ms = millis();
        continue;
      }
      if ((millis() - time_ms) > 100) {
        state = true;
        break;
      }
    }
    return state;
  }
public:
  /**
        Constructor with field
    */
  CustomSerial(HardwareSerial* serial) {
    thisSerial = serial;
    recieving = false;
  }
  void resetArrayData() {
    for (uint_t i = 0; i < LENGTHBUFFERARRAY; i++) {
      arrayData[i] = 0;
    }
  }
  /**
    Procedure serializes data to send as Recieve, sends that dataArray to thisSerial
  */
  void sendReceive (uint8_t comma, uint8_t data, uint8_t length) {
    dataArray[0] = 0x00;
    dataArray[1] = comma;
    dataArray[2] = length;
    for (uint8_t i = 0; i < length; i++) {
      dataArray[i + 3] = data[i];
    }
    sendData(dataArray, length + 3);
  }
  /**
    Procedure serializes data to send as Request, sends that dataArray to thisSerial
  */
  void sendRequest(uint8_t type, uint8_t length) {
    uint8_t dataArray[3] = { 0x01, type, length };
    sendData(dataArray, 3);
  }
  /**
    Procedure serializes data that sends on Request, sends that dataArray to thisSerial
  */
  void sendDataOnRequest(uint8_t* data, uint8_t length) {
    uint8_t dataArray[length + 2];
    dataArray[0] = 0x02;
    dataArray[1] = length;
    for (uint8_t i = 0; i < length; i++) {
      dataArray[i + 2] = data[i];
    }
    sendData(dataArray, length + 2)
  }
  /**
        Procedure that runs in loop()
    */
  void serialListener() {
    if (thisSerial.isAvailable() > 0) {
      dataType = Serial.read();
    }
    if (dataType == 0x02) {
      time_ms = millis();
      while ((millis() - time_ms) > 100) {
        if (Serial.isAvailable() > 0) {
          recieveData()
        }
      }
    } else {
    }
  }
};