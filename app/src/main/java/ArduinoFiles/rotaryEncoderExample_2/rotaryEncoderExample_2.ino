// KY-040 Rotary Encoder Multi-Turn Counter
// CLK → Pin 2 (interrupt 0)
// DT  → Pin 3
// SW  → Pin 4 (button to reset)

volatile long encoderTicks = 0;  // Total accumulated ticks (multi-turn)
const byte CLK = 2;
const byte DT = 3;
const byte SW = 4;

void setup() {
  Serial.begin(115200);
  Serial.println("KY-040 Multi-Turn Encoder Counter");
  Serial.println("Rotate to count ticks. Press button to reset.");

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(CLK), handleRotation, CHANGE);
}

void loop() {
  static long lastTicks = 0;

  // Check if button is pressed (active LOW)
  if (digitalRead(SW) == LOW) {
    delay(10); // Simple debounce
    if (digitalRead(SW) == LOW) {
      encoderTicks = 0;
      Serial.println("\n>>> Counter RESET to 0 <<<");
      lastTicks = 0;
      while (digitalRead(SW) == LOW); // Wait for release
    }
  }

  // Print only when value changes
  if (encoderTicks != lastTicks) {
    Serial.print("Ticks: ");
    Serial.print(encoderTicks);

    // Optional: Calculate full turns (assuming 20 steps per rotation)
    const int STEPS_PER_ROTATION = 20;
    long fullTurns = encoderTicks / STEPS_PER_ROTATION;
    int remainder = encoderTicks % STEPS_PER_ROTATION;

    Serial.print(" | Turns: ");
    Serial.print(fullTurns);
    Serial.print(" + ");
    Serial.print(remainder);
    Serial.println("/20");

    lastTicks = encoderTicks;
  }
}

void handleRotation() {
  // Read DT when CLK changes
  if (digitalRead(CLK) == digitalRead(DT)) {
    encoderTicks++;
  } else {
    encoderTicks--;
  }
}
