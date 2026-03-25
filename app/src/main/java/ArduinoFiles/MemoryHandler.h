#include <EEPROM>
class MemoryHandler {
public:
    /**
        Returns a pointer to array of uint8_t, which numberOfBytes values readed from EEPROM on eepromAdress 
    */
    static uint8_t* readBytesFromEEPROM(uint32_t eepromAddress, uint8_t numberOfBytes) {
        int* array = new int[numberOfBytes];
        for (int i = 0; i < numberOfBytes; i++) {
            EEPROM.get(eepromAddress + i, array[i]);
        }
        return array;
    }
    /**
        Procedure that writes numberOfBytes values from an array to EEPROM on eepromAdress
    */
    static void writeBytesToEEPROM(uint32_t eepromAddress, uint8_t numberOfBytes, uint8_t* array) {
        for (int i = 0; i < numberOfBytes; i++) {
            EEPROM.update(eepromAddress + i, array[i]);
        }
    }
}