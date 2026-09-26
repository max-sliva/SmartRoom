#include <EEPROM.h>
#include <Arduino.h>
class MemoryHandler {
public:
    /**
        Returns a pointer to array of uint8_t, which numberOfBytes values readed from EEPROM on eepromAdress 
    */
    static uint8_t* readBytesFromEEPROM(uint32_t eepromAddress, uint32_t numberOfBytes) {
        uint8_t* array = new uint8_t[numberOfBytes];
        Serial.println();
        for (int i = 0; i < numberOfBytes; i++) {
            EEPROM.get(eepromAddress + i, array[i]);
            Serial.print(array[i]);
            Serial.print(" ");
        }
        Serial.println();
        return array;
    }
    /**
        Procedure that writes numberOfBytes values from an array to EEPROM on eepromAdress
    */
    static void writeBytesToEEPROM(uint32_t eepromAddress, uint8_t numberOfBytes, uint8_t* array) {
      Serial.println();
        for (int i = 0; i < numberOfBytes; i++) {
            EEPROM.update(eepromAddress + i, array[i]);
            Serial.print(array[i]);
            Serial.print(" ");
        }
      Serial.println();
    }
};
