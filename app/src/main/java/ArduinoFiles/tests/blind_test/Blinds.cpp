#include "Arduino.h"
#include <EEPROM.h>
#include "RotaryEncoder.h"

#include "Blinds.h"

Blinds::Blinds(uint8_t dtpin, uint8_t clkpin, uint8_t upMotor, uint8_t downMotor, int down_pos, int up_pos, int cur_pos,  uint16_t bindAddr) {
  bindMemAddress = bindAddr;
  EEPROM.put(bindMemAddress,cur_pos);
  DT_pin = dtpin;
  CLK_pin = clkpin;
  encoder = new RotaryEncoder(DT_pin,CLK_pin);
  encoder->setPosition((long)cur_pos);
  up_motor_pin = upMotor;
  down_motor_pin = downMotor;
  down_value = down_pos;
  up_value = up_value;
}

Blinds::Blinds(uint8_t dtpin, uint8_t clkpin, uint8_t upMotor, uint8_t downMotor, int down_pos, int up_pos, uint16_t bindAddr) {
  bindMemAddress = bindAddr;
  int position_from_eeprom;
  EEPROM.get(bindMemAddress,position_from_eeprom);
  DT_pin = dtpin;
  CLK_pin = clkpin;
  encoder = new RotaryEncoder(DT_pin,CLK_pin);
  encoder->setPosition((long)position_from_eeprom);
  up_motor_pin = upMotor;
  pinMode(upMotor,OUTPUT);
  down_motor_pin = downMotor;
  pinMode(downMotor,OUTPUT);
  down_value = down_pos;
  up_value = up_value;
}

void Blinds::moveUpUntil(int value) {
  Serial.println("Move up...");
  if (value < ((int)encoder->getPosition())) {
    digitalWrite(up_motor_pin,HIGH);
    digitalWrite(down_motor_pin,LOW);
    int pos = (int)encoder->getPosition();
    int newPos;
    while (pos != value) {
      encoder->tick();
      newPos = encoder->getPosition();
      if (pos != newPos) {
        Serial.print("new pos = ");
        Serial.println(newPos);
        pos = newPos;
      } 
    }
    digitalWrite(up_motor_pin,LOW);
    writePosToMemory();
  }
}

void Blinds::moveDownUntil(int value) {
  Serial.println("Move down...");
  if (value > ((int)encoder->getPosition())) {
    digitalWrite(down_motor_pin,HIGH);
    digitalWrite(up_motor_pin,LOW);
    int pos = (int)encoder->getPosition();
    int newPos;
    while (pos != value) {
      encoder->tick();
      newPos = encoder->getPosition();
      if (pos != newPos) {
        Serial.print("new pos = ");
        Serial.println(newPos);
        pos = newPos;
      } 
    }
    digitalWrite(down_motor_pin,LOW);
    writePosToMemory();
  }
}

void Blinds::moveToValue(int value) {
  Serial.println(value);
  Serial.println((int)encoder->getPosition());
  if (value > ((int)encoder->getPosition())) {
    moveDownUntil(value);
  } else {
    moveUpUntil(value);
  }
}

int Blinds::getCurrentValue() {
  return encoder->getPosition();
}

int Blinds::getDownBound() {
  return down_value;
}

int Blinds::getUpBound() {
  return up_value;
}

void Blinds::writePosToMemory() {
  EEPROM.put(bindMemAddress,((int)encoder->getPosition()));
}

void Blinds::readPosFromMemory() {
  int buffer;
  EEPROM.get(bindMemAddress,buffer);
  encoder->setPosition((long)buffer);
}