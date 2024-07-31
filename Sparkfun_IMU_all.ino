#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <SimpleKalmanFilter.h>

MPU6050 mpu;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container

SimpleKalmanFilter kalmanYaw(1, 1, 0.01); // Adjust the parameters based on your needs

const int encoderPinA = 2; // Adjust based on your setup
const int encoderPinB = 3; // Adjust based on your setup

volatile long encoderTicks = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    mpu.initialize();

    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);

    devStatus = mpu.dmpInitialize();

    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(digitalPinToInterrupt(2)), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.print("DMP Initialization failed (code ");
        Serial.print(devStatus);
        Serial.println(")");
    }
}

void loop() {
    if (!dmpReady) return;

    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);
    mpu.resetFIFO();
    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(euler, &q, &gravity);
    float dmpYaw = euler[0] * 180/M_PI; // Convert to degrees

    float encoderYaw = calculateYawFromEncoder();

    float kalmanYawValue = kalmanYaw.updateEstimate(dmpYaw, encoderYaw);

    Serial.print("DMP Yaw: ");
    Serial.print(dmpYaw);
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

float calculateYawFromEncoder() {
    float yaw = (encoderTicks / 360.0) * 360.0; // Assuming 360 ticks per revolution, adjust as needed
    return yaw;
}

void dmpDataReady() {
    mpuIntStatus = mpu.getIntStatus();
}

const int8_t ENC_STATES[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
