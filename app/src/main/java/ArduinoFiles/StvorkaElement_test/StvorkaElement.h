#include "Arduino.h"
class StvorkaElement {
private:
  byte openPin;     //PWM
  byte closePin;    //PWM
  byte valuePin;    //Analog
  short openedValue;    //0 - 1023
  short closedValue;    //0 - 1023
  short currentValue;   //0 - 1023
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

  void setBoundaries(short _openedValue, short _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
  }

  short updateValue() {
    currentValue = analogRead(valuePin);
    return currentValue;
  }

  short updateValueFine(byte tolerance) {
    short newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > tolerance) {
        currentValue = newValue; 
    }
    return currentValue;
  }

  /**
    Procedure to find Boundaries value, by turning element until it can't in both directions
    and the final user input in the Serial which position is closed
  */
  byte findBoundaries(short interruptionTime, byte speed) {
    if (!Serial) {
      return 1;
    }
    Serial.println("Starting finding boundaries...");
    analogWrite(openPin,speed);
    digitalWrite(closePin,LOW);
    long start = millis();
    short value = currentValue;
    while (true) {
      if (value != updateValueFine(4)) {
        start = millis();
        value = currentValue;
      }
      else {
        // exit point from infinite while()
        if ((millis()-start) == interruptionTime) {
          openedValue = value;
          Serial.print("Value1 = ");
          Serial.println(openedValue);
          break;
        }
      }
    }
    digitalWrite(openPin,LOW);
    analogWrite(closePin,speed);
    start = millis();
    while (true) {
      if (value != updateValueFine(4)) {
        start = millis();
        value = currentValue;
      }
      else {
        // exit point from infinite while()
        if ((millis()-start) == interruptionTime) {
          closedValue = value;
          Serial.print("Value2 = ");
          Serial.println(closedValue);
          break;
        }
      }
    }
    digitalWrite(closePin,LOW);
    Serial.println("This is a closed position? [y/n]");
    while (!(Serial.available()>0)) {
      delay(100);
    }
    if (Serial.read()!='y') {
      short buffer = openPin;
      openPin = closePin;
      closePin = buffer;
      buffer = openedValue;
      openedValue = closedValue;
      closedValue = buffer;
      Serial.println("Adjusting variables:");
      Serial.print("openPin = ");
      Serial.println(openPin);
      Serial.print("closePin = ");
      Serial.println(closePin);
    }
    Serial.print("openedValue = ");
    Serial.println(openedValue);
    Serial.print("closedValue = ");
    Serial.println(closedValue);
    Serial.println("Boundaries was adjusted!");
    return 0;
  }

};
