#ifndef Blinds_h
#define Blinds_h

#include "Arduino.h"
#include <EEPROM.h>
#include "RotaryEncoder.h"

class Blinds {
private:
  /*
    values of uint8_t that represents lower & upper bounds of fully closed & opened position of this respectevly
  */
  int down_value, up_value;

  uint16_t bindMemAddress;
  /*
    pins for RotaryEncoder
  */
  uint8_t DT_pin, CLK_pin;
  /*
    pins for motor Driver, down - that closes this, up - that opens this
  */
  uint8_t down_motor_pin, up_motor_pin;

  RotaryEncoder* encoder;
  /*
    method that writes this.current_pos to EEPROM by bindMemAddress
  */
  void writePosToMemory();
  /*
    method that reads byte value from EEPROM by bindMemAddress to this.current_pos
  */
  void readPosFromMemory();
public:
  Blinds(uint8_t DT_pin, uint8_t CLK_pin, uint8_t up_motor, uint8_t down_motor, int down_pos, int up_pos, int cur_pos, uint16_t bindAddr);
  
  Blinds(uint8_t dtpin, uint8_t clkpin, uint8_t upMotor, uint8_t downMotor, int down_pos, int up_pos, uint16_t bindAddr);

  void moveUpUntil(int value);

  void moveDownUntil(int value);

  void moveToValue(int value);

  int getCurrentValue();

  int getUpBound();

  int getDownBound();
};

#endif