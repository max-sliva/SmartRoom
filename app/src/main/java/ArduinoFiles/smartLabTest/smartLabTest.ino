#include "SmartWindow.h"
int vent1 = 3;
int vent2 = 4;
int motor1 = 5;
int motor2 = 6;
int led = 7;
int windowOpenedVal = 30, windowClosedVal = 900, windowCurVal = 600;
bool windowIsOpened = false;
//bool testMode = true;
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define curSerial Serial
#define mode "TestMode"
#else
#define curSerial Serial1
#define mode "WorkMode"
#endif

SmartWindow windowLeft(motor1, motor2, A0, windowOpenedVal, windowClosedVal);
SmartWindow windowRight;

void windowWork(SmartWindow &myWindow, String key, String val) {
   Serial.println("in windowWork..");
   Serial.print("myWindow.isOpened()");
   Serial.print(" = ");
   Serial.println(myWindow.isOpened());
  Serial.print("myWindow.getCurVal() ");
   Serial.print(" = ");
   Serial.println(myWindow.getCurVal() );
  if (val == "open" && !myWindow.isOpened()) {
    if (myWindow.getCurVal() > myWindow.getOpenedVal()) {
      myWindow.open();
    }
  }
  if (val == "close" && myWindow.isOpened()) {
    Serial.println("in closing 1..");
    if (myWindow.getCurVal() < myWindow.getClosedVal()) {
      Serial.println("in closing 2..");
      myWindow.close();
    }
  }
}

void doCommand(String key, String val) {
  if (key == "vent") {
    digitalWrite(vent1, (val == "on") ? HIGH : LOW);
  } else if (key == "windowLeft") {
    Serial.println("windowLeft work");
    windowWork(windowLeft, key, val);
  } else if (key == "windowRight") {
    windowWork(windowRight, key, val);
  } else if (key == "door") {

  } else if (key == "led") {
    if (val == "on") digitalWrite(led, HIGH);
    else if (val == "off") digitalWrite(led, LOW);
    else analogWrite(led, val.toInt());
  }
}

void setup() {
  pinMode(led, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(vent1, OUTPUT);
  pinMode(vent2, OUTPUT);
  digitalWrite(vent2, LOW);
  digitalWrite(vent1, LOW);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println(mode);
  windowLeft.setCurVal(analogRead(A0));
  if (windowLeft.getCurVal() < windowRight.getOpenedVal()) windowLeft.setOpened(true);
  else windowLeft.setOpened(false);
  Serial.print("windowCurVal");
  Serial.print(" : ");
  Serial.println(windowLeft.getCurVal());
  Serial.print("window status");
  Serial.print(" : ");
  String windowStatus = "";
  if (windowLeft.isOpened()) windowStatus = "opened";
  else windowStatus = "closed";
  Serial.println(windowStatus);
}
void loop() {
  //todo сделать режим настройки, в котором можно менять параметры открытия окон и дверей (запись в EEPROM)
  if (curSerial.available()) {
    //    String str = curSerial.readString();
    String key = curSerial.readStringUntil(':');
    String val = curSerial.readStringUntil('\n');

    Serial.print(key);
    Serial.print(" : ");
    Serial.println(val);
    Serial.println();
    doCommand(key, val);
  }
  windowLeft.setCurVal(analogRead(A0));
  int potValue = analogRead(A0);
  if (abs(windowCurVal - potValue) >= 10) {
    Serial.print("potValue = ");
    Serial.println(potValue);
    windowCurVal = potValue;
      if (windowLeft.getCurVal()>=windowLeft.getClosedVal()) windowLeft.setOpened(false);
      else windowLeft.setOpened(true);
  }
  // if (windowLeft.getCurVal() < windowRight.getOpenedVal()) windowLeft.setOpened(true);
  // else windowLeft.setOpened(false);
  delay(15);
}
