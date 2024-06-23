// Encoder pins
const int encoderPinA = 34;  // Set these pins according to your wiring
const int encoderPinB = 35;

// Encoder variables
volatile long encoderCount = 0;
volatile bool lastEncodedA, lastEncodedB;

// Interrupt service routine for encoder
void IRAM_ATTR encoderISR() {
  bool encodedA = digitalRead(encoderPinA);
  bool encodedB = digitalRead(encoderPinB);
  
  // Determine direction of rotation
  if (encodedA != lastEncodedA || encodedB != lastEncodedB) {
    if (encodedA == encodedB) {
      encoderCount++;
    } else {
      encoderCount--;
    }
  }
  
  // Save the current state for the next comparison
  lastEncodedA = encodedA;
  lastEncodedB = encodedB;
}

void setup() {
  Serial.begin(115200);

  // Setup encoder pins
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  // Attach interrupt to encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), encoderISR, CHANGE);
}

void loop() {
  // Print encoder count
  Serial.print("Encoder count: ");
  Serial.println(encoderCount);
  
  delay(100);
}
