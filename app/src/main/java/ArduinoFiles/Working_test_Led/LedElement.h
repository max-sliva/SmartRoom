#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h
#endif
#include "Arduino.h"
class LedElement
{
private:
  byte value;   //0-255
  byte pin;     //MUST BE PWM
  String name;  //NAME OF THE OBJECT IN THE NEXTION DISPLAY, USING CAREFULLY
public:
  LedElement(byte _pin, String _name = "") 
  : pin(_pin), name(_name)
  {
    pinMode(_pin,OUTPUT);
    pin = _pin;
    name = _name;
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
  byte getPin() {
    return pin;
  }
  void setName(String newName) {
    name = newName;
  }
  String getName() {
    return name;
  }
  /*
  *   Procedure that updates value getted from Nextion display
  */
  byte updateValueNextion(EasyNex* source) {
    if (name == ""){
      return value;
    }
    if (name.length() < 5) {
      name += ".val";
    }
    byte number = source->readNumber(name);
    number = map(number,0,100,0,255);
    value = number;
    return number;
  }
};