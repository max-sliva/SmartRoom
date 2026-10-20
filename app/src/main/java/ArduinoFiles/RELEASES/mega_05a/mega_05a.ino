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

Interface* interfaces[4];
uint32_t time_ms = 0;
boolean d_values[3] = {1,0,0};
boolean powerState = false;

void packageHandler(uint8_t comma, uint8_t data) {
  switch (comma) {
    // COMMAND TO SEND LEDS VALUES TO ARD MINI
    case 0:
      Serial.println("Case 0");
      uint8_t length;
      mySerial.resetDataArray();
      // if (values[3] == 0) {
      //   mySerial.sendPackage(0,values[0]);
      // }
      // else {
      //   mySerial.setDataElem(values[1],0);
      //   mySerial.setDataElem(values[2],1);
      //   mySerial.sendPackageExtra(0,length);
      // }
      break;
    case 9:
      Serial.println("Case 9");
      //d_values[0] = data;
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
        //d_values[i] = mySerial.getDataElem(i);
      }
      break;
    default:
      break;
  }
}

void setup() {
  interfaces[0] = new Interface43(2);
  interfaces[1] = new Interface127(2);
  interfaces[2] = new Interface267(2);
  interfaces[3] = new Interface344(1);

  mySerial.onPackage(packageHandler);
  mySerial.onPackageExtra(packageExtraHandler);

  Serial.begin(115200);     // Must match Nextion baud rate
  myNex.begin(115200);      // NEXTION
  mySerial.begin(115200);   // CUSTOMSERIAL
  usbListener.begin(115200);//
  while (!Serial2);
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
}