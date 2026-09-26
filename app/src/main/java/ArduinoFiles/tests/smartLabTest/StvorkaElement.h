#include "Arduino.h"
class StvorkaElement {
private:
  byte openPin;     //PWM
  byte closePin;    //PWM
  byte valuePin;    //Analog
  short int openedValue;    //0 - 1023
  short int closedValue;    //0 - 1023
  short int currentValue;   //0 - 1023
  bool opened;      //Variable that shows this window is opened or not
  String name;      //NAME OF THE OBJECT IN THE NEXTION DISPLAY, USING CAREFULLY
public:
  StvorkaElement(byte _openPin, byte _closePin, byte _valuePin, String _name = "")
    : openPin(_openPin), closePin(_closePin), valuePin(_valuePin), name(_name)
  {
    pinMode(openPin,OUTPUT);
    pinMode(closePin,OUTPUT);
    pinMode(valuePin,INPUT);
    openedValue = 1023;
    closedValue = 0;
    currentValue = 0;
    opened = false;
  }

  void setBoundaries(short int _openedValue, short int _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
  }

  short int updateValue() {
    currentValue = analogRead(valuePin);
    return currentValue;
  }

  short int updateValueFine(byte tolerance = 4) {
    short int newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > tolerance) {
        currentValue = newValue; 
    }
    return currentValue;
  }

  /**
    Procedure to find Boundaries value, by turning element until it can't in both directions
    and the final user input in the Serial which position is opened
  */
  byte findBoundaries(short int interruptionTime = 300, byte speed) {
    if (!Serial) {
      return 1;
    }
    Serial.println("Starting finding boundaries...");
    analogWrite(speed,openPin);
    long start = millis();
    short int value = currentValue;
    while() {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
      else {
        // exit point from infinite while()
        if (millis()-start == interruptionTime) {
          openedValue = value;
          break;
        }
      }
    }
    analogWrite(0,openPin);
    analogWrite(speed,closePin);
    value = currentValue
    while() {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
      else {
        // exit point from infinite while()
        if (millis()-start == interruptionTime) {
          closedValue = value;
          break;
        }
      }
    }
    return 0;
  }

};
