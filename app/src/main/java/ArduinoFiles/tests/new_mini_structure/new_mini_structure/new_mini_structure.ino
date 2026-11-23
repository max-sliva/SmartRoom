#include <Keypad.h>

boolean value = false;
char keymap[3][4] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' }
};
Keypad numpad(makeKeymap(keymap),new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  pinMode(A2,OUTPUT); //ACCESS
  pinMode(A3,OUTPUT); //LOCKED
  pinMode(A0,INPUT);
  digitalWrite(13,LOW);
  Serial.begin(9600);
  while(!Serial);
  Serial.flush();
  Serial.println("Serial Started\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()>0) {
    int data = Serial.read()-48;
    Serial.print("Received data: ");
    Serial.println(data);
    if (value != (data != 0)) {
      digitalWrite(13,HIGH);
      delay(100);
      digitalWrite(13,LOW);
    }
  }
  if (digitalRead(A0) != value) {
    value = value ^ 1;
    if (value) {
      digitalWrite(A3,HIGH);
      digitalWrite(A2,LOW);
    } else {
      digitalWrite(A3,LOW);
      digitalWrite(A2,HIGH);
    }
    Serial.print("Value has changed: ");
    Serial.println(value); 
  }
  char bufferChar = numpad.getKey();
  if (bufferChar != NO_KEY) {
    Serial.print("Entered Key: ");
    Serial.println(bufferChar);
  }
}
