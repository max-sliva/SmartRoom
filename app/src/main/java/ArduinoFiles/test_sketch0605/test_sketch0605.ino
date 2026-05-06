void setup() {
  // put your setup code here, to run once:
  pinMode(5,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);

  pinMode(A0,INPUT);

  digitalWrite(13,LOW);
  digitalWrite(A4,LOW);
  digitalWrite(A5,LOW);
}

boolean state = false;

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(A0) == HIGH) {
    digitalWrite(A5,HIGH);
    digitalWrite(A4,LOW);
  }
  else {
    digitalWrite(A5,LOW);
    digitalWrite(A4,HIGH);
  }
  if (digitalRead(5) == LOW) state = true;
  if (digitalRead(5) == HIGH && state == true) {
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13, LOW); 
    state = false;
  }
  delay(10);
}
