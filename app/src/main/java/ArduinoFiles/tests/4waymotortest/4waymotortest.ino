int state = 0;

int pinmotorA = 2, pinmotorB = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinmotorA,OUTPUT);
  pinMode(pinmotorB,OUTPUT);
  pinMode(52,OUTPUT);
  digitalWrite(pinmotorA,LOW);
  digitalWrite(pinmotorB,LOW);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Ready to listen commands");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 1) {
    state = Serial.parseInt();
    switch (state) {
      case 0:
        digitalWrite(pinmotorA,LOW);
        digitalWrite(pinmotorB,LOW);
        break;
      case 1:
        digitalWrite(pinmotorA,HIGH);
        digitalWrite(pinmotorB,LOW);
        break;
      case 2:
        digitalWrite(pinmotorA,LOW);
        digitalWrite(pinmotorB,HIGH);
        break;
      case 3:
        digitalWrite(pinmotorA,HIGH);
        digitalWrite(pinmotorB,HIGH);
        break;
    }
    Serial.print("Changed state: ");
    Serial.println(state);
  }
}
