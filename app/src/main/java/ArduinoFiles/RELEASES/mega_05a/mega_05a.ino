/**
  MAIN CONTROLLER: ISKRA ARDUINO MEGA 2560
  INCLUDES:
  NEXTION, CUSTOMSERIAL, FANELEMENT, STVORKAELEMENT, BLINDS
  EEPROM
*/
#define SMARTHOUSE_DEVICE_MEGA
#include <library_for_sh.h>

UsbListener usbListener(&Serial);
CustomSerial mySerial(&Serial2);
EasyNex myNex(Serial3);

uint8_t pageCurrent = 255;
uint8_t bufferInter;
Interface* interfaces[4];
uint32_t time_ms = 0;
boolean d_values[3] = {1,0,0};
boolean powerState = false;

uint16_t findInterface(uint32_t serialNumber) {
  for (uint16_t i = 0; i < 4;i++) {
    if (interfaces[i]->getId()==serialNumber) {
      return i;
    }
  }
  return -1;
}

uint16_t findInterfaceByScreen(uint8_t screenNumber) {
  for (uint8_t i = 0; i < 4;i++) {
    if (interfaces[i]->getScreen()==screenNumber) {
      return i;
    }
  }
  return -1;
}

void doActionFromInterfaceData(uint8_t screenNumber) {
  switch (screenNumber) {
    case 0:
      myNex.writeStr("page page_main_menu");
      break;
    case 1:
      break;
  }
}

void packageHandler(uint8_t comma, uint8_t data) {
  switch (comma) {
    // COMMAND TO SEND LEDS VALUES TO ARD MINI
    case 0:
      Serial.println("Case 0");
      mySerial.resetDataArray();
      if (interfaces[0]->isEquals()) {
        mySerial.sendPackage(0,interfaces[0]->getValue(0));
      }
      else {
        uint8_t length = interfaces[0]->length();
        for (uint8_t i = 0; i < length;i++) {
          mySerial.setDataElem(interfaces[0]->getValue(i),i);
        }
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
  //Interfaces init
  interfaces[0] = new Interface43(2,2);
  interfaces[1] = new Interface127(2,5);
  interfaces[2] = new Interface267(2,3);
  interfaces[3] = new Interface344(1,1);
  // Inners of interfaces init
  ((Interface43)interfaces[0]).setSerial(&mySerial);
  ((Interface127)interfaces[1]).setElement(0,Blinds(31,30,7,6,128));
  ((Interface127)interfaces[1]).setElement(1,Blinds(33,32,9,8,129));
  ((Interface267)interfaces[2]).setElement(0,StvorkaElement(2,3,A4,640,295));
  StvorkaElement bufferS = StvorkaElement(4,5,A5,590,200);
  bufferS.setConsts(200,16);
  ((Interface267)interfaces[2]).setElement(1,bufferS);
  ((Interface344)interfaces[1]).setElement(0,FanElement(12,50));
  // setting performers for receiving packages
  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);
  // Start Serials
  Serial.begin(115200);     // Must match Nextion baud rate
  myNex.begin(115200);      // NEXTION
  mySerial.begin(115200);   // CUSTOMSERIAL
  while (!Serial2);

  interfaces[0]->setValue(255,255);

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
  usbListener.listen();
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
  if (pageCurrent != myNex.currentPageId) {
    pageCurrent = myNex.currentPageId;
  }
}

void trigger1() {
  Serial.print("Updated Interface: ");
  bufferInter = findInterface(myNex.readNumber("va0.val"));
  Serial.println(bufferInter);
  if (bufferInter == 65535) {
    return;
  }
  interfaces[bufferInter]->setValue(myNex.readNumber("va1.val"),
  myNex.readNumber("va2.val"));
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
