#include "Arduino.h"
#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h
#endif
class StvorkaElement {
private:
  byte openPin;     // PWM
  byte closePin;    // PWM
  byte valuePin;    // Analog
  short openedValue;    //0 - 1023;   (+/-)
  short closedValue;    //0 - 1023;   (+/-)
  short currentValue;   //0 - 1023;   (+/-)
  /**
  *   IMPORTANT_COMMENT: 3 VALUES ABOVE CAN HAVE MINUS SIGN, BUT ACTUALLY THIS SIGN 
  *                      INFLUENCED BY VALUE 'swapped' AND USED IN LOGIC, NOT IN MATH
  */
  String name;      // NAME OF THE OBJECT IN THE NEXTION DISPLAY, USING CAREFULLY
  /**
  *   Boolean value that shows which direction do opening of stvorka, 
  *   heavily used in 'move' procedures
  */
  bool swapped;
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
    swapped = false;
  }
  // Set openedValue & closedValue
  void setBoundaries(short _openedValue, short _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
  }
  // Returns value from its potenciometer, that influenced by 'swapped'
  short updateValue() {
    currentValue = analogRead(valuePin);
    if (swapped) currentValue *= -1;
    return currentValue;
  }
  // Returns value from its potenciometer when its different by value 'TOLERANCE', that influenced by 'swapped'
  short updateValueFine() {
    const byte TOLERANCE = 4;
    short newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > TOLERANCE) {
        currentValue = newValue;
        if (swapped) currentValue *= -1; 
    }
    return currentValue;
  }
  /**
  *   Returns raw currentValue
  */
  short getCurrentValue() {
    return currentValue;
  }
  /**
  *  Procedure to find Boundaries value, by turning element until it can't in both directions
  *  and the final user input in the Serial which position is closed
  */
  byte findBoundaries(short interruptionTime, byte speed) {
    if (!Serial) {
      return 1;
    }
    Serial.println("Starting finding boundaries...");
    // turning stvorka to max posible value of 'openedValue'
    analogWrite(openPin,speed);
    digitalWrite(closePin,LOW);
    long start = millis();
    short value = currentValue;
    while (!((millis()-start) >= interruptionTime)) {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
    }
    openedValue = value;
    // turning stvorka to max posible value of 'closedValue'
    digitalWrite(openPin,LOW);
    analogWrite(closePin,speed);
    start = millis();
    while (!((millis()-start) >= interruptionTime)) {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
    }
    closedValue = value;
    digitalWrite(closePin,LOW);
    // part where decides 'swapped' value
    Serial.println("This is a closed position? [y/n]");
    while (!(Serial.available()>0)) {
      delay(100);
    }
    if (Serial.read()!='y') {
      short buffer = openPin;
      openPin = closePin;
      closePin = buffer;
      buffer = openedValue*-1;
      openedValue = closedValue*-1;
      closedValue = buffer;
      swapped = true;
      updateValue();
    }
    else {
      swapped = false;
    }
    Serial.print("openedValue = ");
    Serial.println(openedValue);
    Serial.print("closedValue = ");
    Serial.println(closedValue);
    Serial.println("Boundaries was adjusted!");
    return 0;
  }
  /**
  *   Function that returns number in fraction of open position
  */
  byte getOpenByte(short value) {
    if (checkValueWithinBoundaries(value)) {
      if (swapped) {
        if (value < 0) value *= -1;
        return (byte)map(closedValue - value,closedValue,openedValue,0,255);
      }
      else {
        return (byte)map(value,closedValue,openedValue,0,255);
      }
    }
  }
  /**
  *   Function that returns 'currentValue' of this object in fraction of open position
  */
  byte getCurrentOpenByte() {
    return getOpenByte(currentValue);
  }
  /**
  *   Procedure to move stvorka element to the position readed by its potentiometr
  */
  /*short moveDirectTo(short targetValue, byte speed) {
    if (targerValue != abs(currentValue)) {
      if (checkValueWithinBoundaries(targetValue)) {
        const byte TOLERANCE = 4;
        if (targetValue > currentValue) {
          analogWrite(openPin, speed);
          digitalWrite(closePin, LOW);
        }
        else {
          analogWrite(closePin, speed);
          digitalWrite(openPin, LOW);
        }
        while (abs(targetValue-updateValueFine()) > TOLERANCE);
      }
    }
    return currentValue;
  }*/
  /**
  *   Returns bool which says that value lays between openedValue & closedValue
  */
  /*bool checkValueWithinBoundaries(short value) {
    if (swapped && (value < 0)) {
      value *= -1;
    }
    if ((value >= closedValue) && (value <= openedValue)) {
      return true;
    }
    else {
      return false;
    }
  }*/
  // Metod that turn stvorka to opened position
  void openSrvorka() {
    analogWrite(openPin,150);
    digitalWrite(closePin,LOW);
    while (abs(closedValue-updateValueFine()) > 4 );
    digitalWrite(openPin,LOW);
  }
  // Metod that turn stvorka to closed position
  void closeSrvorka() {
    analogWrite(closePin,150);
    digitalWrite(openPin,LOW);
    while (abs(closedValue-updateValueFine()) > 4 );
    digitalWrite(closePin,LOW);
  }
};
