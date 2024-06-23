#include <Wire.h>
#include "MPU6050.h"

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  unsigned long start, end;

  start = micros();
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  end = micros();

  Serial.print("I2C Read Time (us): ");
  Serial.println(end - start);

  delay(1000);
}
