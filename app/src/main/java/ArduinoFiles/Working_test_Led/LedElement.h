#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h
#endif
#include "Arduino.h"
class LedElement
{
private:
  byte value;   //0-255
  byte pin;     //MUST BE PWM
public:
  LedElement(byte _pin) 
  : pin(_pin)
  {
    pinMode(_pin,OUTPUT);
    pin = _pin;
    value = 0;
  }
  /**
  *   Procedure to analogWrite its value to the its pin
  */
  void updateLed(){
    analogWrite(pin,value);
  }
  void updateLed(byte newValue){
    setValue(newValue);
    updateLed();
  }
  void setValue(byte newValue) {
    value=newValue;
  }
  byte getValue() {
    return value;
  }
  /*
  *   Procedure that updates value getted from Nextion display
  */
  byte updateValueNextion(EasyNex* source, const String name) {
    byte number = source->readNumber(name);
    number = map(number,0,100,0,255);
    value = number;
    return number;
  }
};