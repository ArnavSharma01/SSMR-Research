#include <Wire.h>
#include <MPU6050_tockn.h>  // MPU6050 library
#include <Kalman.h>         // Kalman filter library

MPU6050 mpu6050(Wire);      // MPU6050 object

// Variables for filters
float yaw;                  // Yaw angle in degrees
float yawComplementary;     // Yaw angle using complementary filter
Kalman kalmanYaw;           // Kalman filter object for yaw

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.setGyroOffsets(0, 0, 0);  // Set gyro offsets to zero

  kalmanYaw.setAngle(0);          // Initialize Kalman filter angle to 0

  Serial.println("Gyro offsets: Not available in this library");
}

void loop() {
  mpu6050.update();
  
  // Read raw gyro data
  float gyroX = mpu6050.getGyroX();
  float gyroY = mpu6050.getGyroY();
  float gyroZ = mpu6050.getGyroZ();

  // Calculate yaw using raw gyro data (no filter)
  yaw += gyroZ * 0.01;  // Integration over time

  // Complementary filter for yaw
  float dt = 0.01;      // Time step (adjust as necessary)
  yawComplementary = 0.98 * (yawComplementary + gyroZ * dt) + 0.02 * (mpu6050.getAngleZ());

  // Kalman filter for yaw
  float gyroRate = gyroZ / 131.0;  // Convert raw gyro data to degrees per second
  float yawKalman = kalmanYaw.getAngle(yaw, gyroRate, dt);  // Get filtered yaw angle

  // Print yaw angles
  Serial.print("Yaw (no filter): ");
  Serial.print(yaw);
  Serial.print("\t Yaw (complementary): ");
  Serial.print(yawComplementary);
  Serial.print("\t Yaw (Kalman): ");
  Serial.println(yawKalman);

  delay(500);  // Adjust delay as needed for stable output
}
