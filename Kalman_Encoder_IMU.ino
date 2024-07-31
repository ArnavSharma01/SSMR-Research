#include <Wire.h>
#include <MPU6050.h>
#include <SimpleKalmanFilter.h>

MPU6050 mpu;
SimpleKalmanFilter kalmanYaw(1, 1, 0.01); // Adjust the parameters based on your needs

const int encoderPinA = 2; // Adjust based on your setup
const int encoderPinB = 3; // Adjust based on your setup

volatile long encoderTicks = 0;
float previousYaw = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }
}

void loop() {
  // Read the IMU
  mpu.getRotation(&gx, &gy, &gz);
  float imuYaw = calculateYawFromGyro(gz);

  // Read the encoder
  float encoderYaw = calculateYawFromEncoder();

  // Fuse the data using the Kalman filter
  float kalmanYawValue = kalmanYaw.updateEstimate(imuYaw, encoderYaw);

  // Output the result
  Serial.print("IMU Yaw: ");
  Serial.print(imuYaw);
  Serial.print(" Encoder Yaw: ");
  Serial.print(encoderYaw);
  Serial.print(" Kalman Yaw: ");
  Serial.println(kalmanYawValue);

  delay(100);
}

void encoderISR() {
  static uint8_t state = 0;
  state = (state << 2) | ((digitalRead(encoderPinB) << 1) | digitalRead(encoderPinA));
  encoderTicks += (ENC_STATES[state & 0x0F]);
}

float calculateYawFromGyro(int16_t gz) {
  float gyroYawRate = gz / 131.0; // Assuming the gyro sensitivity is set to 131 LSB/°/s
  float dt = 0.01; // Assuming a loop time of 10ms
  float yaw = previousYaw + gyroYawRate * dt;
  previousYaw = yaw;
  return yaw;
}

float calculateYawFromEncoder() {
  // Assuming 360 ticks per revolution, adjust based on your encoder specs
  float yaw = (encoderTicks / 360.0) * 360.0;
  return yaw;
}

// Array to define encoder state transitions
const int8_t ENC_STATES[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
