#include "LedHandler.h"
LedHandler my_handler(2, new uint8_t[2]{10,11});

boolean butstate = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(A2, INPUT);
  pinMode(12, INPUT_PULLUP);

  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(12) == LOW) butstate = true;
  if (digitalRead(12) == HIGH && butstate == true) {
    uint8_t value = map(analogRead(A2),0,1023,0,255);
    Serial.print("Value = ");
    Serial.println(value);
    my_handler.setValueAll(value,1000);
    butstate = false;
    delay(10);
  }
}
