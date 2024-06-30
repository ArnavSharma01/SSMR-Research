// Example code to calculate yaw change using encoder counts

// Define encoder pins
const int encoderLeftPinA = 2;
const int encoderLeftPinB = 3;
const int encoderRightPinA = 4;
const int encoderRightPinB = 5;

// Variables to store encoder counts
volatile long encoderLeftCounts = 0;
volatile long encoderRightCounts = 0;

// Variables for angle calculation
const float countsPerDegree = 1000.0; // Example: adjust according to your encoder specs

void setup() {
  // Initialize encoder pins as inputs
  pinMode(encoderLeftPinA, INPUT);
  pinMode(encoderLeftPinB, INPUT);
  pinMode(encoderRightPinA, INPUT);
  pinMode(encoderRightPinB, INPUT);

  // Attach interrupts for encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderLeftPinA), encoderLeftISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderRightPinA), encoderRightISR, CHANGE);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Calculate yaw change
  float yawChange = (encoderLeftCounts - encoderRightCounts) / countsPerDegree;

  // Print yaw change to serial monitor
  Serial.print("Yaw Change: ");
  Serial.println(yawChange);

  // Reset encoder counts periodically or as needed
  // encoderLeftCounts = 0;
  // encoderRightCounts = 0;

  delay(100); // Adjust delay as needed
}

// Interrupt service routines for encoder counts
void encoderLeftISR() {
  if (digitalRead(encoderLeftPinB) == HIGH) {
    encoderLeftCounts++;
  } else {
    encoderLeftCounts--;
  }
}

void encoderRightISR() {
  if (digitalRead(encoderRightPinB) == HIGH) {
    encoderRightCounts++;
  } else {
    encoderRightCounts--;
  }
}
