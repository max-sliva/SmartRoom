#include <stdint.h>
#include "Arduino.h"

#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h
#endif
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
  // Private procedure that sets all power pins
  void setPins(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin) {
    openPin = _openPin;
    closePin = _closePin;
    valuePin = _valuePin;
  }
  // Private procedure that sets all pinMode to pins
  void setPinModes() {
    pinMode(openPin,OUTPUT);
    pinMode(closePin,OUTPUT);
    pinMode(valuePin,INPUT);
  }
  /**
  *   Private procedure that turns stvorka to raw targetValue checked out by value from potenciometer
  */
  void moveDirectTo(int16_t targetValue) {
    if (targetValue != updateValue()) {
      uint8_t highPin, lowPin;
      if ((targetValue>currentValue)^(closedValue>openedValue)) {
        highPin = openPin;
        lowPin = closePin;
      }
      else {
        highPin = closePin;
        lowPin = openPin;
      }
      analogWrite(highPin, POWER);
      digitalWrite(lowPin, LOW);
      while (abs(targetValue-updateValue()) > TOLERANCE);
      digitalWrite(highPin, LOW);
    }
  }
public:
  // Default constructor: WITHOUT PINMODES!!!
  StvorkaElement() {
    setPins(0,0,0);
    openedValue = 1023;
    closedValue = 0;
    currentValue = 0;
    POWER = 150;
    TOLERANCE = 16;
  }
  // Constuctor where data reads from EEPROM
  // StvorkaElement(uint32_t eepromAddress) {
  //   uint8_t* args = MemoryHandler::readBytesFromEEPROM(eepromAddress,8);
  //   setConsts(args[0],args[1]);
  //   setPins(args[2],args[3],args[4]);
  //   // putting all 8 bits in openedV from byte 5; putting first 2 bits in openedV from byte 6
  //   openedValue = static_cast<uint16_t>(args[5]) | (static_cast<uint16_t>(args[6] & 0b11) << 8);
  //   // putting last 6 bits in closedV from byte 6; putting first 4 bits in closedV from byte 7
  //   closedValue = static_cast<uint16_t>(args[6] >> 2) | (static_cast<uint16_t>(args[7]) << 6);
  //   setPinModes();
  // }
  // Constructor with fields (Used in initial setup)
  StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin) {
    setPins(_openPin, _closePin, _valuePin);
    setPinModes();
    openedValue = 1023;
    closedValue = 0;
    POWER = 150;
    TOLERANCE = 16;
    updateValue();
  }
  // Set openedValue & closedValue
  void setBoundaries(uint16_t _openedValue, uint16_t _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
  }
  void setConsts(uint8_t _power, uint8_t _tolerance) {
    POWER = _power;
    TOLERANCE = _tolerance;
  }
  // Returns value from its potenciometer
  int16_t updateValue() {
    currentValue = analogRead(valuePin);
    return currentValue;
  }
  // Returns value from its potenciometer when its different by value 'TOLERANCE'
  int16_t updateValueFine() {
    uint16_t newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > TOLERANCE) {
        currentValue = newValue; 
    }
    return currentValue;
  }
  /**
  *   Returns raw currentValue
  */
  uint16_t getCurrentValue() {
    return currentValue;
  }
  uint8_t getCurrentValueByte() {
    return map(currentValue,0,1023,0,255);
  }
  /**
  *   Function that returns 'currentValue' in the form of uint8_t mapped by boundary values
  */
  uint8_t getValueInsideBoundaries(uint16_t value) {
    //value & 0x3FF;     hard cast to 0 - 1023
    return map(value & 0x3FF,0,1023,closedValue,openedValue);
  }
  /**
  *   Function that returns any value type of uint8_t in the form uint16_t mapped by boundary values
  */
  uint16_t getShortValue(uint8_t value) {
    return map(value,0,255,closedValue,openedValue);
  }
  uint8_t updateValueByte() {
    return map(updateValue(),closedValue,openedValue,0,255);
  }
  /**
   *  Returns true if value lays between opened & closed Values of stvorkaElement, else false;
   */
  bool valueInsideBoundaries(uint16_t value) {
      int16_t length1, length2;
      length1 = abs(static_cast<int16_t>(openedValue - value));
      length2 = abs(static_cast<int16_t>(closedValue - value));
      return (abs(openedValue - closedValue) == (length1 + length2));
    }
  /**
  *  Procedure to find Boundaries value, by turning element until it can't in both directions
  *  and the final user input in the Serial which position is closed
  */
  uint8_t findBoundaries(uint16_t interruptionTime) {
    if (!Serial) {
      return 1;
    }
    Serial.println("Starting finding boundaries...");
    // turning stvorka to max physically posible value of 'openedValue'
    analogWrite(openPin,POWER);
    digitalWrite(closePin,LOW);
    long start = millis();
    uint16_t value = updateValue();
    while (!((millis()-start) >= interruptionTime)) {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
    }
    openedValue = value;
    // turning stvorka to max physically posible value of 'closedValue'
    digitalWrite(openPin,LOW);
    analogWrite(closePin,POWER);
    start = millis();
    while (!((millis()-start) >= interruptionTime)) {
      if (value != updateValueFine()) {
        start = millis();
        value = currentValue;
      }
    }
    closedValue = value;
    digitalWrite(closePin,LOW);
    // part where opened and closed position specifies
    Serial.println("This is a closed position? [y/n]");
    while (!(Serial.available()>0)) {
      delay(100);
    }
    if (Serial.read()!='y') {
      uint16_t buffer = openPin;
      setPins(closePin,openPin,valuePin);
      setBoundaries(closedValue,openedValue);
      closeSrvorka();
    }
    Serial.print("openedValue = ");
    Serial.println(openedValue);
    Serial.print("closedValue = ");
    Serial.println(closedValue);
    Serial.println("Boundaries was adjusted!");
    return 0;
  }
  /**
  *   Procedures that turns stvorka to procent of opened position (uint8_t), returns value that actually opened to 
  */
  uint16_t moveRelativelyToByte(uint8_t procentValue) {
    uint16_t transformedValue = getShortValue(procentValue);
    moveDirectTo(transformedValue);
    return transformedValue;
  }
  /**
  *   Procedures that turns stvorka to procent of opened position (uint16_t), returns value that actually opened to 
  */
  uint16_t moveRelativeToShort(uint16_t procentValue) {
    uint16_t transformedValue = getValueInsideBoundaries(procentValue);
    moveDirectTo(transformedValue);
    return transformedValue;
  }
  // Metod that turn stvorka to opened position
  void openSrvorka() {
    //Serial.println("opening...");
    analogWrite(openPin,POWER);
    digitalWrite(closePin,LOW);
    while (abs(openedValue-updateValue()) > TOLERANCE);
    digitalWrite(openPin,LOW);
  }
  // Metod that turn stvorka to closed position
  void closeSrvorka() {
    //Serial.println("closing...");
    analogWrite(closePin,POWER);
    digitalWrite(openPin,LOW);
    while (abs(closedValue-updateValue()) > TOLERANCE);
    digitalWrite(closePin,LOW);
  }
  /*
    Returns representation of this object in type of uint_8t of 8 elem*
  */
  uint8_t* toArray() {
    uint32_t buffer = static_cast<uint32_t>(openedValue) | (static_cast<uint32_t>(closedValue) << 10);
    uint8_t* array = new uint8_t[8] {POWER, TOLERANCE, openPin, closePin, valuePin,
       static_cast<uint8_t>(buffer), static_cast<uint8_t>(buffer >> 8), static_cast<uint8_t>(buffer >> 16)};
    return array;
  }
  // void writeBytesToEEPROM(uint32_t eepromAddress) {
  //   MemoryHandler::writeBytesToEEPROM(eepromAddress,8,toArray());
  // }

};
