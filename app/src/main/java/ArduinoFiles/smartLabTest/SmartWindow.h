#include "Arduino.h"
class SmartWindow {
private:
  int motorPin1;
  int motorPin2;
  int potPin;
  int openedVal;
  int closedVal;
  int curVal;
  bool opened;
public:
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
  }

  bool isOpened() const {
    return opened;
  }

  void setOpened(bool windowIsOpened) {
    opened = windowIsOpened;
  }

  void open() {
    Serial.println("opening window...");
    Serial.print("windowCurVal");
    Serial.print(" : ");
    Serial.println(curVal);
    Serial.print("window openedVal");
    Serial.print(" : ");
    Serial.println(openedVal);
    while (curVal > openedVal) {
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      int potValue = analogRead(potPin);
      if (abs(curVal - potValue) >= 10) {
        Serial.print("potValue = ");
        Serial.println(potValue);
        curVal = potValue;
      }
    }
    opened = 1;
    Serial.print("myWindow.isOpened()");
    Serial.print(" = ");
    Serial.println(SmartWindow::isOpened());
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    Serial.println("window is opened!");
  }

  void close() {
    Serial.println("closing window...");
    while (curVal < closedVal) {
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, HIGH);
      int potValue = analogRead(potPin);
      if (abs(curVal - potValue) >= 10) {
        Serial.print("potValue = ");
        Serial.println(potValue);
        curVal = potValue;
      }
    }
    opened = 0;
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    Serial.println("window is closed!");
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
