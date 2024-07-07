#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Encoder.h>
#include <Kalman.h>

// MPU6050 instance
MPU6050 mpu;

// Encoder instances
Encoder leftEncoder(2, 3);  // Update with your encoder pins
Encoder rightEncoder(4, 5); // Update with your encoder pins

// Kalman filter instance for yaw
Kalman kalmanYaw;

// DMP variables
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

// Encoder variables
long leftEncoderTicks = 0;
long rightEncoderTicks = 0;
double encoderYaw = 0.0;

// Robot parameters
const double wheelBase = 0.15; // Distance between wheels in meters
const double ticksPerRevolution = 360.0; // Encoder ticks per revolution
const double wheelCircumference = 0.2; // Circumference of the wheel in meters

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  
  // Verify connection
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  // Load and configure the DMP
  devStatus = mpu.dmpInitialize();
  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print("DMP Initialization failed (code ");
    Serial.print(devStatus);
    Serial.println(")");
    while (1);
  }

  // Initialize Kalman filter
  kalmanYaw.setAngle(0.0);
}

void loop() {
  if (!dmpReady) return;

  // Check for DMP data ready interrupt (this should happen frequently)
  while (fifoCount < packetSize) {
    fifoCount = mpu.getFIFOCount();
  }

  // Read a packet from FIFO
  mpu.getFIFOBytes(fifoBuffer, packetSize);
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  // Calculate yaw from IMU (in degrees)
  double imuYaw = ypr[0] * 180/M_PI;

  // Calculate encoder yaw
  long newLeftEncoderTicks = leftEncoder.read();
  long newRightEncoderTicks = rightEncoder.read();
  double leftDistance = (newLeftEncoderTicks - leftEncoderTicks) / ticksPerRevolution * wheelCircumference;
  double rightDistance = (newRightEncoderTicks - rightEncoderTicks) / ticksPerRevolution * wheelCircumference;
  double deltaYaw = (rightDistance - leftDistance) / wheelBase * 180 / M_PI;

  encoderYaw += deltaYaw;
  leftEncoderTicks = newLeftEncoderTicks;
  rightEncoderTicks = newRightEncoderTicks;

  // Fuse IMU yaw and encoder yaw using Kalman filter
  double kalmanYawValue = kalmanYaw.getAngle(imuYaw, deltaYaw, 0.01);

  // Print the results
  Serial.print("IMU Yaw: ");
  Serial.print(imuYaw);
  Serial.print("\tEncoder Yaw: ");
  Serial.print(encoderYaw);
  Serial.print("\tKalman Yaw: ");
  Serial.println(kalmanYawValue);

  delay(10);
}
