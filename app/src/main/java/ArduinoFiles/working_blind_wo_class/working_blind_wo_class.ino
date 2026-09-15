#include "RotaryEncoder.h"          // библиотека для энкодера
RotaryEncoder encoder(A2, A3);  // пины подключение энкодера (DT, CLK)

static int pos = 0;  // задаем начальное положение энкодера
static const int FIRST = 0;
static const int SECOND = 40;
int newPos;
boolean stateMotor = false;
boolean butState1 = false, butState2 = false;

void setup() {
  Serial.begin(9600);
  Serial.println(pos);  // выводим на монитор начальное значение
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
}

void turnOnPinUntil(uint8_t pin, int value) {
  if (pos == value) {
    return;
  }
  int dx = 1;
  if (pos < value) {
    dx = dx * -1;
  }
  digitalWrite(pin,HIGH);
  while (pos != value) {
    encoder.tick();
    newPos = encoder.getPosition();
    if (pos != newPos) {
      Serial.println(newPos);
      pos = newPos; 
    }
  }
  digitalWrite(pin,LOW);
}


void loop() {
    if (digitalRead(2) == LOW && !stateMotor) {
      butState1 = true;
      stateMotor = true;
    }
    if (digitalRead(2) == HIGH && butState1) {
      turnOnPinUntil(9,FIRST);
      butState1 = false;
      stateMotor = false;
      Serial.println("TURN ON 9");
    }

    if (digitalRead(3) == LOW && !stateMotor) {
      butState2 = true;
      stateMotor = true;
    }
    if (digitalRead(3) == HIGH && butState2) {
      turnOnPinUntil(10,SECOND);
      butState2 = false;
      stateMotor = false;
      Serial.println("TURN ON 10");
    }
}