#include <Keypad.h>

char keymap[3][4] = {
  { '1', '4', '7', '*' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', '#' }
};
const char password[8] = { '1', '2', '3', '4', 0, 0, 0, 0 };
uint32_t hashPass = 0;
char passwordBuffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
char bufferChar;
const uint8_t passLength = 4;
uint8_t counter = 0, lockCounter = 0;
boolean butState = false, specState = false;
Keypad myKeypad = Keypad(makeKeymap(keymap),
                         new uint8_t[3]{ 4, 9, 12 }, new uint8_t[4]{ 7, 3, 2, 8 }, 3, 4);

/**
  Hash function, returns uint32_t hash value from char* str
*/
uint32_t hashDJB2(char* str) {
  uint32_t hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;  // hash * 33 + c
  }
  return hash;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(5, INPUT_PULLUP);

  hashPass = hashDJB2(password);

  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println(hashPass);
}

boolean getChar() {
  bufferChar = myKeypad.getKey();
  if (bufferChar != NO_KEY) {
    return true;
  }
  return false;
}

void blink(uint8_t pin, uint16_t ms) {
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
}

void resetPasswordBuffer() {
  bufferChar = NO_KEY;
  for (uint8_t i = 0; i < 8; i++) {
    passwordBuffer[i] = 0;
  }
  counter = 0;
  lockCounter = 0;
}
void loop() {
  // put your main code here, to run repeatedly:
  if (getChar()) {
    passwordBuffer[counter++] = bufferChar;
    blink(A4, 100);
    Serial.println(passwordBuffer);
    if (counter >= passLength) {
      if (hashPass == hashDJB2(passwordBuffer)) {
        blink(A4, 1000);
      } else {
        blink(A5, 1000);
      }
      resetPasswordBuffer();
    }
    if (bufferChar == keymap[2][3]) {
      lockCounter++;
      if (lockCounter >= 3) {
        resetPasswordBuffer();
        blink(A5,500);
        blink(A4,100);
      }
    }
    else {
      lockCounter = 0;
    }
  }
  if (digitalRead(5) == LOW) butState = true;
  if (digitalRead(5) == HIGH && butState == true) {
    resetPasswordBuffer();
    blink(A5, 100);
    blink(A4, 100);
    butState = false;
    delay(10);
  }
}
