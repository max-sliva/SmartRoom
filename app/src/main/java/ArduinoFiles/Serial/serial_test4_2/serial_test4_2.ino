uint32_t time_ms;
uint8_t data = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(18, INPUT);
  pinMode(19, INPUT);
  time_ms = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - time_ms > 5000) {
    Serial1.write(data);
    //Serial.print(".");
    time_ms = millis();
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
  }
  if (Serial1.available() > 0) {
    data = Serial1.read();
    //Serial.println(data);
    Serial1.write(data);
    time_ms = millis();
    digitalWrite(12,HIGH);
    delay(100);
    digitalWrite(12,LOW);
  }
}
