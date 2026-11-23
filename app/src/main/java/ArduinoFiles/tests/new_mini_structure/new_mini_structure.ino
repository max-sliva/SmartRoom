boolean value = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial has started");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  value = value ^ 1;
  Serial.println(value);
  pinMode(13,value);
}
