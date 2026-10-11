#include "SingleValueDevice.h"
#include <Arduino.h>

#ifndef StvorkaElement_h
#define StvorkaElement_h

class StvorkaElement : public SingleValueDevice {
private:
  const int POWER = 170;       // VARIABLE THAT USED AS CONSTANT: shows how fast(strong) will be turning motor: analogWrite(anyPin,POWER);
  const int TOLERANCE = 16;   /* VARIABLE THAT USED AS CONSTANT: shows how much different must be 'some' 
                     * values related to values getted from potenciometer: analogRead(valuePin); */
  const int openPin;     // PWM
  const int closePin;    // PWM
  const int valuePin;    // Analog

  int buffer_int;

public:

  StvorkaElement() : SingleValueDevice(267,0,1023), openPin(0),closePin(1),valuePin(A0) {}
  
  StvorkaElement(int _openPin, int _closePin, int _valuePin) : SingleValueDevice(267,0,1023), openPin(_openPin), closePin(_closePin), valuePin(_valuePin) {}

  StvorkaElement(int _openPin, int _closePin, int _valuePin, int openValue, int closeValue) 
  : SingleValueDevice(267,openValue,closeValue), openPin(_openPin), closePin(_closePin), valuePin(_valuePin) {}

  int updateValue() {
    this.value = analogRead(valuePin);
    return this.value;
  }

  bool valueInsideBoundaries(int v) {
    buffer_int = abs(MINVALUE - v) + abs(MAXVALUE - v);
    return abs(MINVALUE - MAXVALUE) == buffer_int;
  }

  void setValue(int v) {
    if (!valueInsideBoundaries(updateValue()) or !valueInsideBoundaries(v)) {
      return;
    }
    if (abs(v-updateValue()) < TOLERANCE) {
      return;
    }
    if (v>this.value) {
      analogWrite(closePin, POWER);
      digitalWrite(openPin, LOW);
      while (abs(targetValue-updateValue()) > TOLERANCE);
      digitalWrite(closePin,LOW);
    } else {
      analogWrite(openPin, POWER);
      digitalWrite(closePin, LOW);
      while (abs(targetValue-updateValue()) > TOLERANCE);
      digitalWrite(openPin,LOW);
    }
  }
  
  void openStvorka() {
    if (!valueInsideBoundaries(updateValue())) {
      return;
    }
    if (abs(MINVALUE-updateValue()) < TOLERANCE) {
      return;
    }
    analogWrite(openPin,POWER);
    digitalWrite(closePin,LOW);
    while (abs(MINVALUE-updateValue()) > TOLERANCE);
    digitalWrite(openPin,LOW);
  }
  
  void closeStvorka() {
    if (!valueInsideBoundaries(updateValue())) {
      return;
    }
    if (abs(MAXVALUE-updateValue()) < TOLERANCE) {
      return;
    }
    analogWrite(closePin,POWER);
    digitalWrite(openPin,LOW);
    while (abs(MAXVALUE-updateValue()) > TOLERANCE);
    digitalWrite(closePin,LOW);
  }
};

#endif