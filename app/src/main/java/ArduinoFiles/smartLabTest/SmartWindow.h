#include "Arduino.h"
class SmartWindow {
private:
  byte motorPin1;
  byte motorPin2;
  byte potPin;
  byte openedVal;
  byte closedVal;
  byte curVal;
  bool opened;
public:
//todo добавить получение статуса (открыто/закрыто)
  const int &getOpenedVal() const {
    return openedVal;
  }

  void setOpenedVal(const int &openedVal) {
    SmartWindow::openedVal = openedVal;
  }

  const int &getClosedVal() const {
    return closedVal;
  }

  void setClosedVal(const int &closedVal) {
    SmartWindow::closedVal = closedVal;
  }

  const int &getCurVal() const {
    return curVal;
  }

  void setCurVal(const int &curVal) {
    SmartWindow::curVal = curVal;
    if (curVal<SmartWindow::closedVal) opened = true;
    else opened = false;
  }

  bool isOpened() const {
    return opened;
  }

  void setOpened(bool windowIsOpened) {
    opened = windowIsOpened;
  }

  void open() {
    const byte TOLARATE_VALUE = 16
    digitalWrite(motorPin1, HIGH);
    //digitalWrite(motorPin2, LOW);
    while (curVal > openedVal) {
      int potValue = analogRead(potPin);
      if (abs(curVal - potValue) >= TOLARATE_VALUE) {
        curVal = potValue;
      }
    }
    opened = 1;
    digitalWrite(motorPin1, LOW);
    //digitalWrite(motorPin2, LOW);
  }

  void moveToValue(byte goalValue) {
    const byte TOLARATE_VALUE = 16;
    if ((curVal - goalValue) < 0) {
      // move to open
    }
    else {
      // move to close
    }
  }

  void close() {
    const byte TOLARATE_VALUE = 16;
    //digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    while (curVal < closedVal) {
      int potValue = analogRead(potPin);
      if (abs(curVal - potValue) >= TOLARATE_VALUE) {
        curVal = potValue;
      }
    }
    opened = 0;
    //digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }

  SmartWindow() {}

  SmartWindow(int motorPin1, int motorPin2, int potPin) {
    SmartWindow::motorPin1 = motorPin1;
    SmartWindow::motorPin2 = motorPin2;
    SmartWindow::potPin = potPin;
  }
  SmartWindow(int motorPin1, int motorPin2, int potPin, int openedVal, int closedVal) {
    SmartWindow::motorPin1 = motorPin1;
    SmartWindow::motorPin2 = motorPin2;
    SmartWindow::potPin = potPin;
    SmartWindow::openedVal = openedVal;
    SmartWindow::closedVal = closedVal;
  }
};
