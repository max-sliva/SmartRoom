/**
  MAIN CONTROLLER: ISKRA ARDUINO MEGA 2560
  INCLUDES:
  NEXTION, CUSTOMSERIAL, FANELEMENT, STVORKAELEMENT, BLINDS
  EEPROM
*/
#define SMARTHOUSE_DEVICE_MEGA
#include <library_for_sh.h>

static UsbListener usbListener(&Serial);
static CustomSerial mySerial(&Serial2);
static EasyNex myNex(Serial3);

static uint8_t pageCurrent = 255;
static uint8_t bufferInter;
static Interface* interfaces[4];
static uint32_t time_ms = 0;
static boolean d_values[3] = {1,0,0};
static boolean powerState = false;

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
      myNex.writeNum("h0.val", interfaces[0]->getValue(0));
      myNex.writeNum("h1.val", interfaces[3]->getValue(0));
      myNex.writeStr("t0.txt=\"" + String(map(interfaces[0]->getValue(0),0,255,0,100)) + "\"");
      myNex.writeStr("t1.txt=\"" + String(map(interfaces[3]->getValue(0),0,255,0,100)) + "\"");
      break;
    default:
      bufferInter = findInterfaceByScreen(screenNumber);
      if (bufferInter == 65535) {
        return;
      }
      if (interfaces[bufferInter]->isEquals()) {
        myNex.writeNum("h0.val",interfaces[bufferInter]->getValue(0));
      } else {
        myNex.writeNum("va1.val", 0);
        myNex.writeNum("h1.val",interfaces[bufferInter]->getValue(0));
        myNex.writeNum("h2.val",interfaces[bufferInter]->getValue(1));
        myNex.writeStr("h0.bco=36415");
        myNex.writeStr("h1.bco=50712");
        myNex.writeStr("h2.bco=50712");
        myNex.writeStr("h0.bco1=1055");
        myNex.writeStr("h1.bco1=31727");
        myNex.writeStr("h2.bco1=31727");
      }
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
/*
  SETUP FOR ARDUINO MEGA:
  ====================================================================
  1. Init Interfaces in interfaces array
  2. Add package performers for CustomSerial
  3. Start Serial that are used by myNex, mySerial, usbSerial - and wait until they all launched
  4. Add all elements for every Interface in interfaces:
  4.1. Get downcasted pointers of Interface
  4.2. Add Element for every downcasted pointer
  5. Set pinModes for pins that execute the duty for this Arduino Mega
  6. Set time for time_ms
  ====================================================================
*/
void setup() {
  // init interfaces
  interfaces[0] = new Interface43(2,2);
  interfaces[1] = new Interface127(2,5);
  interfaces[2] = new Interface267(2,3);
  interfaces[3] = new Interface344(1,1);

  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);

  Serial.begin(115200);     // Usb Serial - Must match Nextion baud rate
  Serial2.begin(115200);    // Serial for connection to Arduino Mini by UART
  Serial3.begin(115200);    // Serial for NEXTION
  while (!(Serial && Serial2 && Serial3));
  Serial.println("Serial was started");  
  
  Interface43* i43 = static_cast<Interface43*>(interfaces[0]);
  i43->setSerial(&mySerial);
  i43->setValue(255,255);
  Interface127* i127 = static_cast<Interface127*>(interfaces[1]);
  i127->setElement(0,Blinds(31,30,7,6,128));
  i127->setElement(1,Blinds(33,32,9,8,129));
  Interface267* i267 = static_cast<Interface267*>(interfaces[2]);
  i267->setElement(0,StvorkaElement(2,3,A4,640,295));
  i267->setElement(1,StvorkaElement(4,5,A5,590,200,200,16));
  Interface344* i344 = static_cast<Interface344*>(interfaces[3]);
  i344->setElement(0,FanElement(12,50));
  Serial.println("Interfaces were filled");

  pinMode(A0,INPUT);    // READ STATE OF POWER SWITCH PIN
  pinMode(52,OUTPUT);   // RELAY EXTERNAL POWER PIN
  pinMode(53,OUTPUT);   // RELAY POWER RESET PIN
  pinMode(13,OUTPUT);   // INDICATOR FOR ARDUINO MEGA BOARD ACTIVATION
  powerState = false;

  time_ms = millis();
}

void loop() {
  myNex.NextionListen();
  usbListener.usbListen();
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
    delay(100);
    myNex.writeStr("page page_main_menu");
  }
  if (pageCurrent != myNex.currentPageId) {
    pageCurrent = myNex.currentPageId;
    Serial.print("---> On page: ");
    Serial.println(pageCurrent);
  }
}

void trigger1() {
  Serial.print("Updated Interface: ");
  bufferInter = findInterface(myNex.readNumber("va0.val"));
  Serial.print(bufferInter);
  if (bufferInter == 65535) {
    return;
  }
  uint8_t index = myNex.readNumber("va1.val");
  uint8_t value = map(myNex.readNumber("va2.val"),0,100,0,255);
  Serial.print(" -> ");
  Serial.print(index);
  Serial.print(" ");
  Serial.println(value);
  interfaces[bufferInter]->setValue(index,value);
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
