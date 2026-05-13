/**
  MAIN CONTROLLER: ISKRA ARDUINO MEGA 2560
  INCLUDES:
  NEXTION, CUSTOMSERIAL, FANELEMENT
*/

#include "EasyNextionLibrary.h"
#include "FanElement.h"
#include "CustomSerial.h"

#define RX2 16 

EasyNex myNex(Serial3);
CustomSerial mySerial(&Serial2);
FanElement fan(12,50);

const char* names[10] = {"h0.val","h1.val","h2.val","va0.val",
  "h0.bco","h0.bco1","h1.bco","h1.bco1","h2.bco","h2.bco1"};
uint16_t colors[4] = {36415,1055,50712,31727};
uint8_t values[4] = {0xFF,0xFF,0xFF,0};

boolean extPower = false;
uint8_t colNum;
uint32_t time_ms = 0;

void updateTextLabelsNextion() {
  String buffer;
  if (values[0] > 0) {
    buffer = String(map(values[0],0,255,0,100)) + "\%"; 
  }
  else {
    buffer = "off";
  }
  myNex.writeStr("t0.txt=\"" + buffer + "\"");
  if (fan.getValue() > 10) {
    buffer = String(map(fan.getValue(),0,255,0,100)) + "\%"; 
  }
  else {
    buffer = "off";
  }
  myNex.writeStr("t1.txt=\"" + buffer + "\"");
}

void packageHandler(uint8_t comma, uint8_t data) {
  switch (comma) {
    // COMMAND TO SEND LEDS VALUES TO ARD MINI
    case 0:
      uint8_t length;
      mySerial.resetDataArray();
      if (values[3] == 0) {
        mySerial.sendPackage(0,values[0]);
      }
      else {
        mySerial.setDataElem(values[1],0);
        mySerial.setDataElem(values[2],1);
        mySerial.sendPackageExtra(0,length);
      }
      break;
    default:
      break;
  }
}

void packageExtraHandler(uint8_t comma, uint8_t length) {
  switch (comma) {
    case 0:
      analogWrite(13,255);
      delay(500);
      digitalWrite(13,LOW);
      delay(100);
      if (mySerial.getDataElem(0) > 0) {
        analogWrite(13,255);
        delay(500);
        digitalWrite(13,LOW);
        delay(100);
      }
      if (mySerial.getDataElem(1) > 0) {
        analogWrite(13,100);
        delay(500);
        digitalWrite(13,LOW);
      }
      break;
    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // Must match Nextion baud rate
  myNex.begin(115200);      // NEXTION

  mySerial.onPackage(packageHandler);
  mySerial.onPackage(packageExtraHandler);

  pinMode(A0,INPUT);    // READ STATE OF POWER SWITCH PIN
  pinMode(52,OUTPUT);   // RELAY EXTERNAL POWER PIN
  pinMode(53,OUTPUT);   // RELAY POWER RESET PIN
  pinMode(13,OUTPUT);
  // pinMode(31,OUTPUT);

  digitalWrite(53,LOW);
  digitalWrite(52,HIGH);
  myNex.writeStr("page page_main_menu");
  
  time_ms = millis();
}

void loop() {
  myNex.NextionListen();
  mySerial.serialListen();
  if (digitalRead(A0) == LOW) {
    digitalWrite(52,LOW);
    delay(1000);
    digitalWrite(53, HIGH);
  }
  if ((myNex.currentPageId==1)&&((millis() - time_ms) >= 1000)) {
    time_ms = millis();
    myNex.writeNum(names[0],values[0]);
    myNex.writeNum(names[1],fan.getValue());
    updateTextLabelsNextion();
  }
  if ((extPower == false)&&(analogRead(A5) > 200)) {
    extPower = true;
    mySerial.begin(115200);
  }
  if ((extPower == true)&&(analogRead(A5) < 200)) {
    extPower = false;
    mySerial.end();
  }
}

void trigger0() {
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
  // установка значения переменных к значению на nextion h0,h1,h2,
  // где имена и значения хранятся в массивах до 4 
  for (uint8_t i = 0; i < 4; i++) {
    myNex.writeNum(names[i],values[i]);
  }
}

void trigger1() {
  values[0] = myNex.readNumber(names[0]);
  mySerial.sendPackage(0,values[0]);
}

void trigger2() {
  uint8_t value;
  mySerial.resetDataArray();
  for (uint8_t i = 0; i < 2; i++) {
    value = myNex.readNumber(names[i+1]);
    if (value != values[i+1]) {
      values[i+1] = value;
      mySerial.setDataElem(i, 0);
      mySerial.setDataElem(value, 1);
      mySerial.sendPackageExtra(1, 2);
      break;
    }
  }
  values[0] = round((values[1]+values[2])/2);
}

void trigger3() {
  fan.setAccuratePWMValue(myNex.readNumber(names[1]));
}