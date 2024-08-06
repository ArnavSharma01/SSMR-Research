#include <Wire.h>
#include <MPU9250.h>

MPU9250 mpu;
unsigned long previousTime = 0;
float yawAngle = 0.0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpu.setup(0x68)) {  // Change to 0x69 if needed
    Serial.println("MPU9250 connection failed");
    while (1);
  }

  // Initialize timer
  previousTime = millis();
}

void loop() {
  mpu.update();

  // Get gyroscope values in degrees per second
  float gX = mpu.getGyroX();
  float gY = mpu.getGyroY();
  float gZ = mpu.getGyroZ();

  // Calculate time difference
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  // Integrate gyroscope z-axis to get yaw angle
  yawAngle += gZ * deltaTime;

  // Print yaw angle
  Serial.print("Yaw: ");
  Serial.println(yawAngle);

  delay(10);
}
