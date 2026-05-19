uint32_t time_ms;
uint8_t data;

void setup() {
  // put your setup code here, to run once:
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  Serial1.begin(115200);
  time_ms = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - time_ms > 1000) {
    time_ms = millis();
    data = millis() % 256;
    Serial.print("S: ");
    Serial.println(data);
    Serial1.write(data);
  }
  if (Serial1.available() > 0) {
    Serial.print("G: ");
    Serial.println(Serial1.read());
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
  }
}
