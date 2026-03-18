#include "StvorkaElement.h"

class PreciseStvorkaElement : public StvorkaElement {
protected:
    /**
      Private procedure that turns stvorka to raw targetValue checked out by value from potenciometer
    */
    void moveDirectlyTo(int8_t targetValue) {
        if (targetValue != getValue()) {
            uint8_t highPin, lowPin;
        if ((targetValue>currentValue)^(closedValue>openedValue)) {
            highPin = PINS[0];
            lowPin = PINS[1];
        }
        else {
            highPin = PINS[1];
            lowPin = PINS[0];
        }
        analogWrite(highPin, POWER);
        digitalWrite(lowPin, LOW);
        while (abs(targetValue-getValue()) > TOLERANCE);
        digitalWrite(highPin, LOW);
    }
  }
public:
    // Terribly similar functions which easily can get confusing
    /**
        Returns uint8_t which corresponds to procentage of opened position by Boundaries
    */
    uint8_t getProcentOfOpening(uint8_t number) {
        return map(number,closedValue,openedValue,0,255);
    }
    /**
        Returns uint8_t which corresponds to Boundaries by procentage of opened position 
    */
    uint8_t getNumberInsideBounderies(uint8_t number) {
        return map(number,0,255,closedValue,openedValue);
    }
    /**
       Procedures that turns stvorka to procent of opened position (uint8_t), returns value that actually opened to 
    */
    uint8_t moveRelativelyTo(uint8_t procentValue) {
        uint8_t transformedValue = getNumberInsideBounderies(procentValue);
        moveDirectlyTo(transformedValue);
        return transformedValue;
  }
};