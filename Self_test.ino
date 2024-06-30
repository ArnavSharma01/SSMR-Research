#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  mpu.initialize();
  
  // Verify connection to MPU6050
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  // Trigger self-test
  uint8_t selfTestResult = mpu.testSelfTest();
  
  // Read accelerometer and gyroscope values during self-test
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Print results to Serial Monitor
  Serial.print("Self-Test Result: ");
  Serial.println(selfTestResult, HEX);
  
  Serial.print("Accelerometer (raw) - X:");
  Serial.print(ax); Serial.print(" Y:");
  Serial.print(ay); Serial.print(" Z:");
  Serial.println(az);
  
  Serial.print("Gyroscope (raw) - X:");
  Serial.print(gx); Serial.print(" Y:");
  Serial.print(gy); Serial.print(" Z:");
  Serial.println(gz);
  
  delay(1000); // Delay between readings
}
