uint8_t dataContr[2] = {8,2};
uint8_t pinLeds[2] = {10,11};
uint8_t ledData[2] = {0xFF,0xFF};

void receiveEvent() {
  uint8_t dataFunc[3];
  uint8_t i = 0;
  while(Wire.available()) {   // Пока есть данные
    dataFunc[i] = Wire.read(); // Получить 1 байт
    i++;
    if (i > 3) break;
  }
  switch (dataFunc[0]) {
    case 0:
      setLedValue(dataFunc[1],dataFunc[2]);
      break;
    case 1:
      break;
  }
}

void setLedValue(uint8_t pinIndex, uint8_t new_value) {
  if (pinIndex < dataContr[1]) {
    analogWrite(pinLeds[pinIndex],new_value);
    ledData[pinIndex] = new_value;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  
  Wire.begin(dataContr[0]);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}
