#ifndef STVORKAELEMENT_H
#define STVORKAELEMENT_H

#include <stdint.h>
#include <Arduino.h>

class StvorkaElement {
private:
  uint8_t POWER;       // VARIABLE THAT USED AS CONSTANT: shows how fast(strong) will be turning motor: analogWrite(anyPin,POWER);
  uint8_t TOLERANCE;   /* VARIABLE THAT USED AS CONSTANT: shows how much different must be 'some' 
                     * values related to values getted from potenciometer: analogRead(valuePin); */
  uint8_t openPin;     // PWM
  uint8_t closePin;    // PWM
  uint8_t valuePin;    // Analog
  uint16_t openedValue;    //0 - 1023;
  uint16_t closedValue;    //0 - 1023;
  uint16_t currentValue;   //0 - 1023;

  /**
    Sets pins for openning, closing, and getting current possition value (analog)
  */
  void setPins(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin);

  /**
  *   Private procedure that turns stvorka to raw targetValue checked out by value from potenciometer
  */
  void moveDirectTo(int16_t targetValue);
public:

  /// Default constructor
  StvorkaElement();

  /// Constructor with fields: PINS
  StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin);

  /// Consturctor with fields: PINS AND BOUNDARIES
  StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin, uint16_t _openedValue, uint16_t _closedValue);

  /// Constructor will all fields
  StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin, uint16_t _openedValue, uint16_t _closedValue, uint8_t _power, uint8_t _tolerance);

  /**
    Initilisates all pins and all subAction on the start
    Needed to call before working in begin()
  */
  void init();

  /// Set openedValue & closedValue
  void setBoundaries(uint16_t _openedValue, uint16_t _closedValue);

  void setConsts(uint8_t _power, uint8_t _tolerance);

  /// Returns value from its potenciometer
  int16_t updateValue();

  /// Returns value from its potenciometer when its different by value 'TOLERANCE'
  int16_t updateValueFine();

  /// Returns raw currentValue
  uint16_t getCurrentValue();

  /// Function that returns 'currentValue' in the form of uint8_t mapped by boundary values
  uint8_t getValueInsideBoundaries(uint16_t value);

  /// Function that returns any value type of uint8_t in the form uint16_t mapped by boundary values
  uint16_t getShortValue(uint8_t value);

  /// Returns current value that mapped by Boundaries between 0 and 255
  uint8_t getByteValue();

  /** 
    Returns a value that mapped by Boundaries between 0 and 255 
  */
  // THIS IS NOT A STATIC METHOD
  uint8_t getByteValue(uint16_t value);

  /// Returns true if value lays between opened & closed Values of stvorkaElement, else false;
  bool valueInsideBoundaries(int16_t value);

  /**
  *   Procedures that turns stvorka to procent of opened position (uint8_t), returns value that actually opened to 
  */
  uint16_t moveRelativelyToByte(uint8_t procentValue);

  /**
  *   Procedures that turns stvorka to procent of opened position (uint16_t), returns value that actually opened to 
  */
  uint16_t moveRelativeToShort(uint16_t procentValue);

  // Metod that turn stvorka to opened position

  void openStvorka();
  
  // Metod that turn stvorka to closed position
  void closeStvorka();
};

#endif