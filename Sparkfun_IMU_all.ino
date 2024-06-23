void setup() {
  Serial.begin(115200); // Start the serial communication with the computer
  Serial2.begin(57600, SERIAL_8N1, 16, 17); // Start serial communication with Razor IMU (TX pin, RX pin)
}

void loop() {
  static unsigned long lastPrint = 0;
  unsigned long now = millis();
  
  // Check if data is available to read from Razor IMU
  while (Serial2.available()) {
    char c = Serial2.read(); // Read the incoming data from Razor IMU
    
    // Check for the start of a data packet
    if (c == '$') {
      // Read the following characters until a newline or carriage return is encountered
      String data = Serial2.readStringUntil('\n');
      
      // Example: $RPY,12.34,-56.78,90.12*45
      // Parse the data if it starts with $RPY
      if (data.startsWith("$RPY")) {
        // Remove the $RPY prefix
        data.remove(0, 4);
        
        // Split the remaining string by commas
        int comma1 = data.indexOf(',');
        int comma2 = data.indexOf(',', comma1 + 1);
        
        if (comma1 != -1 && comma2 != -1) {
          // Extract accelerometer, gyro, and magnetometer values
          float accelX = data.substring(0, comma1).toFloat();
          float accelY = data.substring(comma1 + 1, comma2).toFloat();
          float accelZ = data.substring(comma2 + 1).toFloat();
          
          // Print accelerometer data
          Serial.print("Accel X: "); Serial.print(accelX, 6);
          Serial.print(", Y: "); Serial.print(accelY, 6);
          Serial.print(", Z: "); Serial.println(accelZ, 6);
        }
      }
    }
  }
  
  // Print data every second
  if (now - lastPrint >= 1000) {
    // Request data from Razor IMU
    Serial2.println("REQ"); // Example command to request data
    lastPrint = now;
  }
}

