#include <HardwareSerial.h>
#include <Arduino.h>
#include <EEPROM.h>

class UsbListener {
private:

  HardwareSerial* _serial = nullptr;

  void (*sendLambda)(int,int) = nullptr;

public:

  UsbListener(HardwareSerial serial) {
    _serial = &serial; 
  }

  void begin(uint32_t baud) {
    if (_serial!=nullptr) {
      _serial->begin(baud);
    }
  }

  void end() {
    if (_serial!=nullptr) {
      _serial->end();
    }
  }

  void setDifferentPerformer(void (*function)(int,int)) {
    sendLambda = function;
  }

  void usbListen() {
    if (_serial != nullptr) {
      if (_serial->available() > 3) {
        uint8_t data[3];
        data[0] = _serial->parseInt();
        data[1] = _serial->parseInt();
        data[2] = _serial->parseInt();
        if (data[0] == 0) {
          EEPROM.update(data[1],data[2]);
        } else if (data[0] == 1) {
          if (sendLambda != nullptr) {
            sendLambda(data[1],data[2]);
          }
        }
      }
    }
  }

};