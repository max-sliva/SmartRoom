#include <Wire.h>

boolean butstate = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(A15, INPUT);
  
  digitalWrite(13, LOW);

  Serial.begin(9600);
  Wire.begin();
  Serial.println("Serial Started\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(2) == LOW) butstate = true;
  if ((digitalRead(2) == HIGH) && (butstate == true)) {
    Wire.beginTransmission(8);
    Wire.write(map(analogRead(A15),0,1023,0,255));
    Wire.endTransmission();

    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);

    butstate = false;
  }
  delay(15);
}
