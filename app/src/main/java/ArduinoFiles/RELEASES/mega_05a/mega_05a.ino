/**
  MAIN CONTROLLER: ISKRA ARDUINO MEGA 2560
  INCLUDES:
  NEXTION, CUSTOMSERIAL, FANELEMENT, STVORKAELEMENT, BLINDS
  EEPROM
*/
#define SMARTHOUSE_DEVICE_MEGA
#include <library_for_sh.h>
#include <EEPROM.h>

EasyNex myNex(Serial3);
CustomSerial mySerial(&Serial2);

Interface* interfaces;
uint32_t time_ms = 0;
boolean d_values[3] = {1,0,0};
boolean powerState = false;
/**
  Used for writing distinct values into eeprom of main(case 0) or secondary(case1):

  example:
  "0 127 255" - writes to main controller on 127 address value 255
  "1 25 20" - writes to secondary controller on 25 address value 20
*/
void listenUsbSerial() {
  if (Serial.available() > 3) {
    uint8_t data[3];
    data[0] = Serial.parseInt();
    data[1] = Serial.parseInt();
    data[2] = Serial.parseInt();
    if (data[0] == 0) {
      EEPROM.update(data[1],data[2]);
      Serial.println("Wrote on First controller");
    } else if (data[0] == 1) {
      mySerial.setDataElem(data[1],0);
      mySerial.setDataElem(data[2],1);
      mySerial.sendPackageExtra(127,2);
      Serial.println("Wrote on Second controller");
    }
  }
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
  interfaces = new Interface[4];
  interfaces[0] = new Interface43(2);
  interfaces[1] = new Interface127(2);
  interfaces[2] = new Interface267(2);
  interfaces[3] = new Interface344(1);
}

void loop() {

}