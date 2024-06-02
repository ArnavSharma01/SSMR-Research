#include <HardwareSerial.h>

// Define the pins for UART communication
#define RXD2 16
#define TXD2 17
#define OTR_PIN 4 // Example pin for OTR, modify as per your connection

HardwareSerial mySerial(2); // Use UART2 for communication

void setup() {
  // Start the built-in Serial Monitor for debugging
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // Setup UART2 communication
  mySerial.begin(57600, SERIAL_8N1, RXD2, TXD2); // Set your baud rate

  // Configure OTR pin if needed
  pinMode(OTR_PIN, OUTPUT);
  digitalWrite(OTR_PIN, HIGH); // Set OTR to HIGH (or LOW depending on your sensor's requirement)
  
  Serial.println("ESP32 IMU UART Communication Initialized");
}

void loop() {
  // Check if data is available to read
  if (mySerial.available()) {
    // Read data from the IMU sensor
    String imuData = mySerial.readStringUntil('\n'); // Assuming data ends with a newline character
    Serial.println("IMU Data: " + imuData);
  }
  
  // Add a small delay to avoid overwhelming the Serial buffer
  delay(100);
}
