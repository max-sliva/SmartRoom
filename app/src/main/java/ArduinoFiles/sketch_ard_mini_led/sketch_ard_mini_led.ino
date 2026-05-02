void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT_PULLUP);
  pinMode(A2,INPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

}

boolean butstate = false, statepwm = false;

uint8_t value1 = 0, value2 = 0;
uint16_t newValue1, newValue2; 

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(7) == LOW) butstate = true;
  if ((digitalRead(7) == HIGH) && butstate == true) {
    statepwm = !statepwm;
    butstate = false;
  }
  if (statepwm == true) {
    newValue1 = map(analogRead(A2),0,1023,0,255);
    if (abs(newValue1 - value1) > 4) {
      analogWrite(10,newValue1);
      value1 = newValue1;
    }
  }
  else {
    newValue2 = map(analogRead(A2),0,1023,0,255);
    if (abs(newValue2 - value2) > 4) {
      analogWrite(11,newValue2);
      value2 = newValue2;
    }
  }
  delay(5);
}
