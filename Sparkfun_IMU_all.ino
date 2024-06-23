#include <Wire.h>
#include <SparkFunMPU9250-DMP.h> // Include SparkFun MPU-9250-DMP library

// Pin definitions
#define IMU_RESET_PIN 14   // GPIO pin connected to DTR pin on SEN-10736
#define IMU_RX_PIN 16      // GPIO pin connected to TX0 pin on SEN-10736
#define IMU_TX_PIN 17      // GPIO pin connected to RXI pin on SEN-10736
#define IMU_BAUDRATE 57600 // Baud rate of the SEN-10736

// Create an instance of the MPU9250-DMP class
MPU9250_DMP imu;

void setup() {
  Serial.begin(115200); // Start serial communication with ESP32
  Wire.begin(); // Start I2C communication (needed for MPU9250 library)

  // Initialize the SEN-10736 IMU
  imu.begin(IMU_BAUDRATE, IMU_RX_PIN, IMU_TX_PIN, IMU_RESET_PIN);
  
  // Verify communication with the IMU
  if (imu.begin() != INV_SUCCESS) {
    Serial.println("Failed to communicate with MPU-9250!");
    while (1); // Halt the program if communication fails
  }
  
  // Initialize the IMU sensor fusion and set the output data rate
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);
}

void loop() {
  // Read sensor data
  imu.update();

  // Get accelerometer, gyroscope, and magnetometer data
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;

  imu.getAccel(&ax, &ay, &az);
  imu.getGyro(&gx, &gy, &gz);
  imu.getMag(&mx, &my, &mz);

  // Print sensor data
  Serial.print("Acc (m/s^2): ");
  Serial.print(ax); Serial.print(", ");
  Serial.print(ay); Serial.print(", ");
  Serial.println(az);

  Serial.print("Gyro (deg/s): ");
  Serial.print(gx); Serial.print(", ");
  Serial.print(gy); Serial.print(", ");
  Serial.println(gz);

  Serial.print("Mag (uT): ");
  Serial.print(mx); Serial.print(", ");
  Serial.print(my); Serial.print(", ");
  Serial.println(mz);

  delay(100); // Delay between readings
}
