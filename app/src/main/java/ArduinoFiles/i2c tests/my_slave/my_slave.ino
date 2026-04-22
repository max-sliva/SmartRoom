#include <Wire.h>

void receiveEvent() {
  analogWrite(11, Wire.read());
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(11,OUTPUT);
  pinMode(13,OUTPUT);

  digitalWrite(13,LOW);

  Serial.begin(9600);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.println("Slave Started\n");
}

void loop() {
  delay(50);
}
