#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize I2C communication
  Wire.begin();

  // Initialize the MPU6050
  Serial.println("Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    while (1) { }
  }

  // Set up the MPU6050 with default settings
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 initialized successfully!");
}

void loop() {
  // Read accelerometer values
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print accelerometer values
  Serial.print("Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Accel Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Accel Z: ");
  Serial.println(a.acceleration.z);

  // Print gyroscope values
  Serial.print("Gyro X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Gyro Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Gyro Z: ");
  Serial.println(g.gyro.z);

  // Print temperature
  Serial.print("Temp: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  // Add a delay
  delay(500);
}
