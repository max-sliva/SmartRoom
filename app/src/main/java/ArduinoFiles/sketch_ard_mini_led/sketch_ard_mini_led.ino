void setup() {
  // put your setup code here, to run once:
  pinMode(A2,INPUT);
  pinMode(10,OUTPUT);
}

uint8_t value = 0;
uint16_t newValue; 

void loop() {
  // put your main code here, to run repeatedly:
  newValue = map(analogRead(A2),0,1023,0,255);
  if (abs(newValue - value) > 4) {
    analogWrite(10,newValue);
    value = newValue;
  }
  delay(5);
}
