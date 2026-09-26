#include <EEPROM.h>

static const int BINDADDR = 128;
static const int VALUE = 0;

void setup() {
  pinMode(13,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Serial Started");
  EEPROM.put(BINDADDR,VALUE);
  Serial.print("Wrote: ");
  Serial.print(VALUE);
  Serial.print(" on: ");
  Serial.println(BINDADDR);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}
