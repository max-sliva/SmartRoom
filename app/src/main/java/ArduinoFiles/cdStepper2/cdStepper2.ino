#include <Stepper.h>

// change this to fit the number of steps per revolution for your motor
const int stepsPerRevolution = 200;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
char c = '0';
void setup() {
  // set the speed at x rpm:
  myStepper.setSpeed(50);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    c = Serial.read();
    if (c == '1') {
      Serial.println("clockwise");
      myStepper.step(stepsPerRevolution);
    }
    if (c == '2') {
      Serial.println("counterclockwise");
      myStepper.step(-stepsPerRevolution);
    }
  }
  // step one revolution in one direction:
  //  Serial.println("clockwise");
  //  myStepper.step(stepsPerRevolution);
  //  delay(5000);
  //
  //  // step one revolution in the other direction:
  //  Serial.println("counterclockwise");
  //  myStepper.step(-stepsPerRevolution);
  //  delay(5000);
}
