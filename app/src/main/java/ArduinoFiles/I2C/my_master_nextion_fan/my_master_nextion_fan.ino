#include "EasyNextionLibrary.h"
#include "Wire.h"

#include "FanElement.h"

EasyNex myNex(Serial3);  // Using Hardware Serial
FanElement fan(12,50);

const char* names[10] = {"h0.val","h1.val","h2.val","va0.val",
  "h0.bco","h0.bco1","h1.bco","h1.bco1","h2.bco","h2.bco1"};
uint16_t colors[4] = {36415,1055,50712,31727};
uint8_t values[4] = {0,0,0,0};

uint8_t colNum;

void sendFunction(uint8_t comma, uint8_t data1, uint8_t data2, uint8_t addres) {
  byte data[3] = {comma, data1, data2};

  Wire.beginTransmission(addres);
  Wire.write(data, sizeof(data));
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);   // Must match Nextion baud rate
  myNex.begin(115200);    // NEXTION
  Wire.begin();           // I2C

  pinMode(A0,INPUT);    // READ STATE OF POWER SWITCH PIN
  pinMode(52,OUTPUT);   // RELAY EXTERNAL POWER PIN
  pinMode(53,OUTPUT);   // RELAY POWER RESET PIN
  pinMode(13,OUTPUT);

  digitalWrite(53,LOW);
  digitalWrite(52,HIGH);
  myNex.writeStr("page page_main_menu");
}

void loop() {
  myNex.NextionListen();
  if (digitalRead(A0) == LOW) {
    delay(1000);
    digitalWrite(53, HIGH);
  }
}

void trigger0() {

  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);

  // установка цветовой палитры
  if (values[1] == values[2]) {
    values[0] = values[1];
    values[4] = 0;
    colNum = 0;
  }
  else {
    values[4] = 1;
    colNum = 2;
  }
  // colNum переменная нужна для 
  for (uint8_t i = 4; i < 10; i++) {
    myNex.writeNum(names[i],colors[colNum]);
    if ((i < 6) || (i % 2 == 1)) colNum++;
    if (colNum > 3) colNum = 0;
  }
  // установка значения переменных к значению на nextion h0,h1,h2, где имена и значения хранятся в массивах до 4 
  for (uint8_t i = 0; i < 4; i++) {
    myNex.writeNum(names[i],values[i]);
  }
}

void trigger1() {
  values[0] = myNex.readNumber(names[0]);
  sendFunction(0,values[0],0,8);
}

void trigger2() {
  uint8_t value = myNex.readNumber(names[1]);
  if (value != values[1]) {
    values[1] = value;
    sendFunction(1,0,values[1],8);
  }
  else {
    values[2] = myNex.readNumber(names[2]);
    sendFunction(1,1,values[2],8);
  }
}

void trigger3() {
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
  fan.setAccuratePWMValue(myNex.readNumber(names[1]));
}

void trigger4() {
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
  
}