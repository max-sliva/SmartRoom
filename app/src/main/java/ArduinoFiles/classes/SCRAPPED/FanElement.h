#include "SingleValueObject.h"
#include <Arduino.h>

#ifndef FanElement_h
#define FanElement_h

class FanElement : public SingleValueObject {
private:
  const int pin;
public:
  FanElement() : SingleValueObject(344,0,255), pin(0) {
  }
  FanElement(int _pin, int _minPWMvalue) : SingleValueObject(344,_minPWMvalue,255), pin(_pin) {
    pinMode(_pin, OUTPUT);
  }

  void setValue(int v) {
    this.value = getAccuratePWMValue(v);
    analogWrite(this.pin,this.value);
  }

  int getAccuratePWMValue(int v) {
    if (v == 0) return 0;
    else return map(v,1,255,MINVALUE,MAXVALUE);
  }

  int getValue() {
    if (this.value == 0) return 0;
    else return map(this.value,MINVALUE,MAXVALUE,1,255);
  }

  int getRawValue() {
    return this.value;
  }

  void setValueOverTime(int v, long ms) {
    if ((this.value-v) < 0) dx = -1;
    else dx = 1;
    long ms_dx = ms / abs(this.value-v)  
    for (int i = this.value; i != v; i+=dx) {
      setValue(i);
      delay(ms_dx);
    }
  }
    
  int updateValue() {
    setValue(this.value);
    return getValue();
  }
};

#endif