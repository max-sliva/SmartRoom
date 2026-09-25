/**
  MAIN CONTROLLER: ISKRA ARDUINO MEGA 2560
  INCLUDES:
  NEXTION, CUSTOMSERIAL, FANELEMENT, STVORKAELEMENT, BLINDS
*/

#include "EasyNextionLibrary.h"
#include "FanElement.h"
#include "CustomSerial.h"
#include "StvorkaElement.h"
#include "Blinds.h"

EasyNex myNex(Serial3);
CustomSerial mySerial(&Serial2);
FanElement fan(12,50);
StvorkaElement windows[2];

const char* names[10] = {"h0.val","h1.val","h2.val","va0.val",
  "h0.bco","h0.bco1","h1.bco","h1.bco1","h2.bco","h2.bco1"};
const char* w_names[3] = {"h1.val","h2.val","h0.val"};
//uint16_t colors[4] = {36415,1055,50712,31727};
uint16_t blue_palitra[2] = {36415,1055};
uint16_t gray_palitra[2] = {50712,31727};
uint8_t values[4] = {0xFF,0xFF,0xFF,0};
boolean d_values[3] = {1,0,0};
boolean powerState = false;
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
      Serial.println("Case 0");
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
    case 9:
      Serial.println("Case 9");
      d_values[0] = data;
      break;
    case 10:
      Serial.println("Case 10");
      d_values[2] = true;
      if (myNex.currentPageId==4) {
        myNex.writeStr("vis t0,1");
        myNex.writeStr("vis b5,1");
        myNex.writeStr("vis b6,1");
      }
      break;
    case 11:
      Serial.println("Case 11");
      d_values[2] = false;
      if (myNex.currentPageId==4) {
        myNex.writeStr("vis t0,0");
        myNex.writeStr("vis b5,0");
        myNex.writeStr("vis b6,0");
      }
    default:
      break;
  }
}

void packageExtraHandler(uint8_t comma, uint8_t length) {
  switch (comma) {
    case 0:
      for (uint8_t i = 0; i < 3;i++) {
        d_values[i] = mySerial.getDataElem(i);
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
  mySerial.begin(115200);   // CUSTOMSERIAL
  while (!Serial2);

  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  
  //INIT WINDOWS STVORKAS
  windows[0] = StvorkaElement(2,3,A4);
  windows[0].setBoundaries(640, 295);
  windows[1] = StvorkaElement(4,5,A5);
  windows[1].setBoundaries(590, 200);
  windows[1].setConsts(200, 16);

  pinMode(A0,INPUT);    // READ STATE OF POWER SWITCH PIN
  pinMode(52,OUTPUT);   // RELAY EXTERNAL POWER PIN
  pinMode(53,OUTPUT);   // RELAY POWER RESET PIN
  pinMode(13,OUTPUT);
  powerState = true;

  digitalWrite(53,LOW);
  digitalWrite(52,HIGH);
  myNex.writeStr("page page_main_menu");
  
  time_ms = millis();
}

void loop() {
  myNex.NextionListen();
  mySerial.serialListen();
  if (digitalRead(A0) == LOW && powerState == true) {
    powerState = false;
    Serial.println("Power turn off");
    digitalWrite(52,LOW);
    delay(1000);
    digitalWrite(53, HIGH);
  }
  if (digitalRead(A0) == HIGH && powerState == false) {
    powerState = true;
    Serial.println("Power turn on");
    digitalWrite(53, LOW);
    digitalWrite(52, HIGH);
  }
  if ((myNex.currentPageId==1)&&((millis() - time_ms) >= 1000)) {
    Serial.println("Update Labels");
    time_ms = millis();
    myNex.writeNum(names[0],values[0]);
    myNex.writeNum(names[1],fan.getValue());
    updateTextLabelsNextion();
  }
}

void trigger0() {
  for (uint8_t i = 0; i < 4; i++) {
    myNex.writeNum(names[i],values[i]);
  }
  // установка цветовой палитры
  // РАБОТАЕТ НЕ КОРРЕКТНО
  // if (values[1] == values[2]) {
  //   values[0] = values[1];
  //   values[4] = 0;
  //   colNum = 0;
  // }
  // else {
  //   values[4] = 1;
  //   colNum = 2;
  // }
  // // colNum переменная нужна для 
  // for (uint8_t i = 4; i < 10; i++) {
  //   myNex.writeNum(names[i],colors[colNum]);
  //   if ((i < 6) || (i % 2 == 1)) colNum++;
  //   if (colNum > 3) colNum = 0;
  // }
  if ((values[1] == values[2]) && (values[2] == values[0])) {
    myNex.writeNum("h0.bco", blue_palitra[0]);
    myNex.writeNum("h0.bco1", blue_palitra[1]);
    myNex.writeNum("h1.bco", gray_palitra[0]);
    myNex.writeNum("h1.bco1", gray_palitra[1]);
    myNex.writeNum("h2.bco", gray_palitra[0]);
    myNex.writeNum("h2.bco1", gray_palitra[1]);
  }
  else {
    myNex.writeNum("h0.bco", gray_palitra[0]);
    myNex.writeNum("h0.bco1", gray_palitra[1]);
    myNex.writeNum("h1.bco", blue_palitra[0]);
    myNex.writeNum("h1.bco1", blue_palitra[1]);
    myNex.writeNum("h2.bco", blue_palitra[0]);
    myNex.writeNum("h2.bco1", blue_palitra[1]);
  }
}
// all lights
void trigger1() {
  if (myNex.readNumber(names[0]) == values[0]) {
    Serial.println("one light change");
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
  }
  else {
    Serial.println("All light change");
    values[0] = myNex.readNumber(names[0]);
    values[1] = values[0];
    values[2] = values[0];
    mySerial.sendPackage(0,values[0]);
  }
}
// OLD TRIGGERS
// void trigger1() {
//   values[0] = myNex.readNumber(names[0]);
//   mySerial.sendPackage(0,values[0]);
// }
// // distinct light (only 1 at a time)
// void trigger2() {
//   uint8_t value;
//   mySerial.resetDataArray();
//   for (uint8_t i = 0; i < 2; i++) {
//     value = myNex.readNumber(names[i+1]);
//     if (value != values[i+1]) {
//       values[i+1] = value;
//       mySerial.setDataElem(i, 0);
//       mySerial.setDataElem(value, 1);
//       mySerial.sendPackageExtra(1, 2);
//       break;
//     }
//   }
//   values[0] = round((values[1]+values[2])/2);
// }
// set fun
void trigger3() {
  fan.setAccuratePWMValue(myNex.readNumber(names[1]));
}

void trigger4() {
  Serial.println("Update door interface");
  myNex.writeNum("va0.val", d_values[1]);
  if (d_values[2] == HIGH) {
    myNex.writeStr("vis t0,1");
    myNex.writeStr("vis b5,1");
    myNex.writeStr("vis b6,1");
  }
}

void trigger5() {
  Serial.println("Update door");
  if (myNex.readNumber("va1.val")!=d_values[0]) {
    d_values[0] = !d_values[0];
    mySerial.sendPackage(10, d_values[0]);
  } 
}

void trigger6() {
  Serial.println("Update Locker");
  if (myNex.readNumber("va2.val")!=d_values[1]) {
    d_values[1] = !d_values[1];
    mySerial.sendPackage(11, d_values[1]); 
  }
}

void trigger7() {
  Serial.println("Request answer");
  if (myNex.readNumber("va2.val")!=d_values[2]) {
    d_values[2] = !d_values[2];
    mySerial.sendPackage(12, d_values[2]);
  }
}

void trigger9() {
  Serial.println("Update windows interface");
  for (uint8_t i = 0; i < 2; i++) {
    windows[i].updateValue();
  }
  uint8_t value1 = windows[0].getByteValue(windows[0].getCurrentValue()),
  value2 = windows[1].getByteValue(windows[1].getCurrentValue());

  if (abs(value1 - value2) > 4) {
    myNex.writeNum(w_names[2],(
      value1 + value2) / 2);
    myNex.writeNum("va0.val",1);
    myNex.writeNum("h0.bco", gray_palitra[0]);
    myNex.writeNum("h0.bco1", gray_palitra[1]);
    myNex.writeNum("h1.bco", blue_palitra[0]);
    myNex.writeNum("h1.bco1", blue_palitra[1]);
    myNex.writeNum("h2.bco", blue_palitra[0]);
    myNex.writeNum("h2.bco1", blue_palitra[1]);
  }
  else {
    myNex.writeNum(w_names[2],value1);
    myNex.writeNum("va0.val", 0); 
    myNex.writeNum("h0.bco", blue_palitra[0]);
    myNex.writeNum("h0.bco1", blue_palitra[1]);
    myNex.writeNum("h1.bco", gray_palitra[0]);
    myNex.writeNum("h1.bco1", gray_palitra[1]);
    myNex.writeNum("h2.bco", gray_palitra[0]);
    myNex.writeNum("h2.bco1", gray_palitra[1]);
  }
  myNex.writeNum(w_names[0],value1);
  myNex.writeNum(w_names[1],value2);
}

void trigger10() {
  Serial.println("Distinct windows update");
  uint8_t selectedW = myNex.readNumber("va2.val");
  if (selectedW != 255) {
    windows[selectedW].moveRelativelyToByte(myNex.readNumber(w_names[selectedW]));
  } else {
    for (uint8_t i = 0; i < 2; i++) {
      windows[i].moveRelativelyToByte(myNex.readNumber("h0.val"));
    }
  }
}