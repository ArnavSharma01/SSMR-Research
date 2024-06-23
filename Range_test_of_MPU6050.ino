#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    if (!mpu.begin()) {
        Serial.println("MPU6050 initialization failed!");
        while (1);
    }

    // Set accelerometer and gyroscope ranges
    //mpu.setAccelerometerRange(MPU6050_RANGE_2_G);   // ±2g
    //mpu.setGyroRange(MPU6050_RANGE_250_DEG);        // ±250°/s

    // Uncomment the following lines to test other ranges:
     //mpu.setAccelerometerRange(MPU6050_RANGE_4_G);   // ±4g
     //mpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // ±8g
     mpu.setAccelerometerRange(MPU6050_RANGE_16_G);  // ±16g

     //mpu.setGyroRange(MPU6050_RANGE_500_DEG);        // ±500°/s
    // mpu.setGyroRange(MPU6050_RANGE_1000_DEG);       // ±1000°/s
     mpu.setGyroRange(MPU6050_RANGE_2000_DEG);       // ±2000°/s
}

void loop() {
    float accelSumX = 0, accelSumY = 0, accelSumZ = 0;
    float gyroSumX = 0, gyroSumY = 0, gyroSumZ = 0;

    for (int i = 0; i < 1000; i++) {
        sensors_event_t accel, gyro, temp;
        mpu.getEvent(&accel, &gyro, &temp);

        accelSumX += accel.acceleration.x;
        accelSumY += accel.acceleration.y;
        accelSumZ += accel.acceleration.z;

        gyroSumX += gyro.gyro.x;
        gyroSumY += gyro.gyro.y;
        gyroSumZ += gyro.gyro.z;

        delay(1); // Small delay between readings
    }

    // Calculate averages
    float accelAvgX = accelSumX / 1000.0;
    float accelAvgY = accelSumY / 1000.0;
    float accelAvgZ = accelSumZ / 1000.0;

    float gyroAvgX = gyroSumX / 1000.0;
    float gyroAvgY = gyroSumY / 1000.0;
    float gyroAvgZ = gyroSumZ / 1000.0;

    Serial.println("Accelerometer Averages (m/s^2):");
    Serial.print("X=");
    Serial.print(accelAvgX);
    Serial.print(", Y=");
    Serial.print(accelAvgY);
    Serial.print(", Z=");
    Serial.println(accelAvgZ);

    Serial.println("Gyroscope Averages (rad/s):");
    Serial.print("X=");
    Serial.print(gyroAvgX);
    Serial.print(", Y=");
    Serial.print(gyroAvgY);
    Serial.print(", Z=");
    Serial.println(gyroAvgZ);

    delay(1000); // Adjust as needed
}
