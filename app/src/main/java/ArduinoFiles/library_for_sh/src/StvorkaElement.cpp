#include "StvorkaElement.h"

void StvorkaElement::setPins(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin) {
    openPin = _openPin;
    closePin = _closePin;
    valuePin = _valuePin;
}

void StvorkaElement::moveDirectTo(int16_t targetValue) {
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

StvorkaElement::StvorkaElement()
    : StvorkaElement(0,0,0,1023,0,150,16) {
}

StvorkaElement::StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin) 
    : StvorkaElement(_openPin, _closePin, _valuePin, 1023, 0, 150, 16) {
}

StvorkaElement::StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin, uint16_t _openedValue, uint16_t _closedValue)
    : StvorkaElement(_openPin, _closePin, _valuePin, _openedValue, _closedValue, 150, 16) {
}

StvorkaElement::StvorkaElement(uint8_t _openPin, uint8_t _closePin, uint8_t _valuePin, uint16_t _openedValue, uint16_t _closedValue, uint8_t _power, uint8_t _tolerance) {
    setPins(_openPin, _closePin, _valuePin);
    setBoundaries(_openedValue,_closedValue);
    setConsts(_power,_tolerance);
}

void StvorkaElement::init() {
    pinMode(openPin,OUTPUT);
    pinMode(closePin,OUTPUT);
    pinMode(valuePin,INPUT);
    updateValue();
}

void StvorkaElement::setBoundaries(uint16_t _openedValue, uint16_t _closedValue) {
    openedValue = _openedValue;
    closedValue = _closedValue;
}

void StvorkaElement::setConsts(uint8_t _power, uint8_t _tolerance) {
    POWER = _power;
    TOLERANCE = _tolerance;
}

int16_t StvorkaElement::updateValue() {
    currentValue = analogRead(valuePin);
    return currentValue;
}

int16_t StvorkaElement::updateValueFine() {
    uint16_t newValue = analogRead(valuePin);
    if (abs(newValue - currentValue) > TOLERANCE) {
        currentValue = newValue; 
    }
    return currentValue;
}

uint16_t StvorkaElement::getCurrentValue() {
    return currentValue;
}

uint8_t StvorkaElement::getValueInsideBoundaries(uint16_t value) {
    //value & 0x3FF;     hard cast to 0 - 1023
    return map(value & 0x3FF,0,1023,closedValue,openedValue);
}

uint16_t StvorkaElement::getShortValue(uint8_t value) {
    return map(value,0,255,closedValue,openedValue);
}

uint8_t StvorkaElement::getByteValue() {
    return getByteValue(updateValue);
}

// THIS IS NOT A STATIC METHOD
uint8_t StvorkaElement::getByteValue(uint16_t value) {
    return map(value,closedValue,openedValue,0,255);
}

bool StvorkaElement::valueInsideBoundaries(int16_t value) {
    int16_t length1, length2;
    length1 = abs((int16_t)openedValue - value);
    length2 = abs((int16_t)closedValue - value);
    int16_t result = (abs((int16_t)openedValue - (int16_t)closedValue) == (length1 + length2));
    return result;
}

uint16_t StvorkaElement::moveRelativelyToByte(uint8_t procentValue) {
    uint16_t transformedValue = getShortValue(procentValue);
    moveDirectTo(transformedValue);
    return transformedValue;
}

uint16_t StvorkaElement::moveRelativeToShort(uint16_t procentValue) {
    uint16_t transformedValue = getValueInsideBoundaries(procentValue);
    moveDirectTo(transformedValue);
    return transformedValue;
}

void StvorkaElement::openStvorka() {
    if (valueInsideBoundaries(updateValue())) {
        analogWrite(openPin, POWER);
        digitalWrite(closePin, LOW);
        while (abs((int16_t)openedValue - updateValue()) > TOLERANCE);
        digitalWrite(openPin, LOW);
    }
}

void StvorkaElement::closeStvorka() {
    if (valueInsideBoundaries(updateValue())) {
        analogWrite(closePin, POWER);
        digitalWrite(openPin, LOW);
        while (abs((int16_t)closedValue - updateValue()) > TOLERANCE);
        digitalWrite(closePin, LOW);
    }
}