#include "Arduino.h"
#include <EEPROM.h>
#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h
#endif
class StvorkaElement {
private:
  byte POWER;       // VARIABLE THAT USED AS CONSTANT: shows how fast(strong) will be turning motor: analogWrite(anyPin,POWER);
  byte TOLERANCE;   /* VARIABLE THAT USED AS CONSTANT: shows how much different must be 'some' 
                     * values related to values getted from potenciometer: analogRead(valuePin); */
  byte openPin;     // PWM
  byte closePin;    // PWM
  byte valuePin;    // Analog
  short openedValue;    //0 - 1023;
  short closedValue;    //0 - 1023;
  short currentValue;   //0 - 1023;
  // Private procedure that sets all power pins
  void setPins(byte _openPin, byte _closePin, byte _valuePin) {
    openPin = _openPin;
    closedPin = _closePin;
    valuePin = _valuePin;
  }
  // Private procedure that sets all pinMode to pins
  void setPinModes() {
    pinMode(openPin,OUTPUT);
    pinMode(closePin,OUTPUT);
    pinMode(valuePin,INPUT);
  }
public:
  // Default constructor: WITHOUT PINMODES!!!
  StvorkaElement() {
    setPins(0,0,0);
    openedValue = 1023;
    closedValue = 0;
    currentValue = 0;
    POWER = 150;
    TOLERANCE = 4;
  }
  // Constuctor where data reads from EEPROM
  StvorkaElement(int eepromAddress) {
    readBytesFromEEPROM(eepromAddress);
    setPinModes();
    updateValue();
  }
  // Constructor with fields (Used in initial setup)
  StvorkaElement(byte _openPin, byte _closePin, byte _valuePin) {
    setPins(_openPin, _closePin, _valuePin);
    setPinModes();
    openedValue = 1023;
    closedValue = 0;
    POWER = 150;
    TOLERANCE = 4;
    updateValue();
  }
  // Set openedValue & closedValue
  void setBoundaries(short _openedValue, short _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
  }
  void setConsts(byte _power, byte _tolerance) {
    POWER = _power;
    TOLERANCE = _tolerance;
  }
  // Returns value from its potenciometer
  short updateValue() {
    currentValue = analogRead(valuePin);
    return currentValue;
  }
  // Returns value from its potenciometer when its different by value 'TOLERANCE'
  short updateValueFine() {
    short newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > TOLERANCE) {
        currentValue = newValue; 
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
  *   Function that returns 'currentValue' in the form of byte mapped by boundary values
  */
  byte getCurrentValueByte() {
    return map(currentValue,closedValue,openedValue,0,255);
  }
  /**
  *   Function that returns any value type of byte in the form short mapped by boundary values
  */
  short getShortValue(byte value) {
    return map(value,0,255,closedValue,openedValue);
  }
  /**
  *  Procedure to find Boundaries value, by turning element until it can't in both directions
  *  and the final user input in the Serial which position is closed
  */
  byte findBoundaries(short interruptionTime) {
    if (!Serial) {
      return 1;
    }
    Serial.println("Starting finding boundaries...");
    // turning stvorka to max physically posible value of 'openedValue'
    analogWrite(openPin,POWER);
    digitalWrite(closePin,LOW);
    long start = millis();
    short value = updateValue();
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
      short buffer = openPin;
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
  *   Procedure that turns stvorka to procent of opened position, returns value that actually opened to 
  */
  short moveRelativelyTo(byte procentValue) {
    short transformedValue = getShortValue(procentValue);
    moveDirectTo(transformedValue);
    return transformedValue;
  }
  /**
  *   Procedure that turns stvorka to raw targetValue readed by potenciometer
  */
  void moveDirectTo(short targetValue) {
    if (targetValue != updateValue()) {
      byte highPin, lowPin;
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
      while (abs(openedValue-updateValueFine()) > TOLERANCE);
      digitalWrite(highPin, LOW);
    }
  }
  // Metod that turn stvorka to opened position
  void openSrvorka() {
    //Serial.println("opening...");
    analogWrite(openPin,POWER);
    digitalWrite(closePin,LOW);
    while (abs(openedValue-updateValueFine()) > TOLERANCE);
    digitalWrite(openPin,LOW);
  }
  // Metod that turn stvorka to closed position
  void closeSrvorka() {
    //Serial.println("closing...");
    analogWrite(closePin,POWER);
    digitalWrite(openPin,LOW);
    while (abs(closedValue-updateValueFine()) > TOLERANCE);
    digitalWrite(closePin,LOW);
  }
  void writeBytesToEEPROM(int eepromAddress) {
    //Serial.println("writing to EEPROM...");
    // openedValue & closedValue transformation to 3 bytes for
    // 0-1024 is 10 bits what means that 10 bits of each short (overall 20 bits / 2 bytes & 4 bits) must be located in 3 bytes 
    byte b6 = static_cast<byte>(openedValue), b7, b8;   // put first 8 bits of openedValue in b6 (8 of 20)
    b7 = static_cast<byte>(openedValue >> 8);           // put 2 higher bits of openedValue in the start of b7 (10 of 20)
    b7 = b7 | (static_cast<byte>(closedValue << 2));      // put 6 lower bits of closedValue in the end of b7 (16 of 20)
    b8 = static_cast<byte>(closedValue >> 6);           // put 4 higher bits of closedValue in the start of b8 (20 of 20)
    const byte array[] = {POWER, TOLERANCE, openPin, closedPin, valuePin, b6, b7, b8};
    for (int i = 0; i < 8; i++) {
      EEPROM.update(eepromAddress+i,array[i]);
    }
  }

  void readBytesFromEEPROM(int eepromAddress) {
    //Serial.println("reading from EEPROM...");
    byte array[8];
    for (int i = 0; i < 8; i++) {
      EEPROM.get(eepromAddress+i, array[i]);
    }
    setConsts(array[0],array[1]);
    setPins(array[2],array[3],array[4]);
    short openedV = static_cast<short>(array[5]), closedV;              // put first 8 bits in openedV (8 of 20)
    openedV = openedV | ((static_cast<short>(array[6]) << 8) & 0x300);  // put higher 2 bits in openedV (10 of 20)
    closedV = static_cast<short>(array[6] >> 2);                        // put 6 lower bits in closedV (16 of 20)
    closedV = closedV | (static_cast<short>(array[7]) << 6);            // put 4 higher bits in closedV (20 of 20)
    setBoundaries(openedV,closedV);
  }
};
