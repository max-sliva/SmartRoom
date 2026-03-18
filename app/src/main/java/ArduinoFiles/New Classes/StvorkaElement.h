#include "Element"

class StvorkaElement : public Element {
protected:
    uint8_t openedValue;
    uint8_t closedValue;
    uint8_t POWER;
    uint8_t TOLERANCE;
    /**
        Private procedure that sets all pinMode to pins 
    */
    void setPinModes() {
        pinMode(PINS[0], OUTPUT);
        pinMode(PINS[1], OUTPUT);
        pinMode(PINS[2], ANALOG);
    }
public:
    // Default constructor: WITHOUT PINMODES!!!
    StvorkaElement() {
        setPins(0,0,0);
        setBoundaries(255,0);
        currentValue = 0;
        setConsts(150,4);
    }
    // Constuctor by an array of 7, used together with readBytesFromEEPROM
    StvorkaElement(uint8_t* array) {
        setPins(new uint8_t[] {array[0], array[1], array[2]});
        setBoundaries(array[3], array[4]);
        setConsts(array[5], array[6]);
        setPinModes();
        getValue();
    }
    // Constructor with fields (Used in initial setup)
    StvorkaElement(uint8_t openPin, uint8_t closePin, uint8_t valuePin) {
        setPins(new uint8_t[] {openPin, closePin, valuePin});
        setPinModes();
        setBoundaries(255,0);
        setConsts(150,4);
        getValue();
    }
    // Set openedValue & closedValue
    void setBoundaries(uint8_t _openedValue, uint8_t _closedValue) {
        openedValue = _openedValue;
        closedValue = _closedValue;
    }
    void setConsts(uint8_t _power, uint8_t _tolerance) {
        POWER = _power;
        TOLERANCE = _tolerance;
    }
    uint8_t getValue() {
        uint8_t newValue = map(analogRead(PINS[2]),0,1023,0,255);
        value = newValue;
        return value;
    }
    /**
        Procedure to find Boundaries value, by turning element until it can't in both directions
        and the final user input in the Serial which position is closed
    */
    uint8_t findBoundaries(uint8_t interruptionTime) {
        if (!Serial) {
            return 1;
        }
        Serial.println("Starting finding boundaries...");
        // turning stvorka to max physically posible value of 'openedValue'
        analogWrite(PINS[0],POWER);
        digitalWrite(PINS[1],LOW);
        long start = millis();
        uint8_t oldValue = getValue();
        while (!((millis()-start) >= interruptionTime)) {
            if (abs(oldValue - getValue()) <= TOLERANCE) {
                start = millis();
                oldValue = value;
            }
        }
        openedValue = value;
        // turning stvorka to max physically posible value of 'closedValue'
        digitalWrite(PINS[0],LOW);
        analogWrite(PINS[1],POWER);
        start = millis();
        while (!((millis()-start) >= interruptionTime)) {
            if (abs(oldValue - getValue()) <= TOLERANCE) {
                start = millis();
                oldValue = value;
            }
        }
        closedValue = value;
        digitalWrite(PINS[1],LOW);
        // part where opened and closed position specifies
        Serial.println("This is a closed position? [y/n]");
        while (!(Serial.available()>0)) {
            delay(100);
        }
        if (Serial.read()!='y') {
            uint16_t buffer = PINS[0];
            setPins(new uint8_t[NUMOFPINS] {PINS[1], PINS[0], PINS[2]});
            closeSrvorka();
        }
        Serial.print("openedValue = ");
        Serial.println(openedValue);
        Serial.print("closedValue = ");
        Serial.println(closedValue);
        Serial.println("Boundaries was adjusted!");
        return 0;
    }
    // Metod that turn stvorka to opened position
    void openSrvorka() {
        //Serial.println("opening...");
        analogWrite(PINS[0],POWER);
        digitalWrite(PINS[1],LOW);
        while (abs(openedValue-getValue()) > TOLERANCE);
        digitalWrite(PINS[0],LOW);
    }
    // Metod that turn stvorka to closed position
    void closeSrvorka() {
        //Serial.println("closing...");
        analogWrite(PINS[1],POWER);
        digitalWrite(PINS[0],LOW);
        while (abs(closedValue-getValue()) > TOLERANCE);
        digitalWrite(PINS[1],LOW);
    }
    /**
        Returns a pointer to uint8_t array representation of this object
    */
    uint8_t* toByteArray() {
        return new uint8_t[7] {PINS[0], PINS[1], PINS[2], openedValue, closedValue, POWER, TOLERANCE};
    }
};