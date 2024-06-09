#include <Wire.h>

#define MPU6050_ADDR_1 0x68 // I2C address for the first MPU6050
#define MPU6050_ADDR_2 0x69 // I2C address for the second MPU6050
#define SAMPLING_PERIOD_MS 10
#define COMPLEMENTARY_FILTER_ALPHA 0.98

struct MPU6050Data {
  float accel_x;
  float accel_y;
  float accel_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
};

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050 sensors
  mpu6050_init(MPU6050_ADDR_1);
  mpu6050_init(MPU6050_ADDR_2);

  delay(100); // Allow some time for sensors to initialize
}

void loop() {
  MPU6050Data data1, data2;
  static float angle_x1 = 0, angle_y1 = 0;
  static float angle_x2 = 0, angle_y2 = 0;

  // Read data from both MPU6050 sensors
  mpu6050_read(MPU6050_ADDR_1, &data1);
  mpu6050_read(MPU6050_ADDR_2, &data2);

  // Apply the complementary filter
  complementary_filter(&data1, &angle_x1, &angle_y1);
  complementary_filter(&data2, &angle_x2, &angle_y2);

  // Print the angles
  Serial.print("Sensor 1 - Angle X: "); Serial.print(angle_x1); Serial.print(", Angle Y: "); Serial.println(angle_y1);
  Serial.print("Sensor 2 - Angle X: "); Serial.print(angle_x2); Serial.print(", Angle Y: "); Serial.println(angle_y2);

  delay(SAMPLING_PERIOD_MS);
}

void mpu6050_init(uint8_t address) {
  Wire.beginTransmission(address);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0x00); // Wake up MPU6050
  Wire.endTransmission();
}

void mpu6050_read(uint8_t address, MPU6050Data *data) {
  Wire.beginTransmission(address);
  Wire.write(0x3B); // Starting register for accelerometer and gyro data
  Wire.endTransmission(false);
  Wire.requestFrom(address, (uint8_t)14);

  // Read accelerometer data
  data->accel_x = (int16_t)((Wire.read() << 8) | Wire.read());
  data->accel_y = (int16_t)((Wire.read() << 8) | Wire.read());
  data->accel_z = (int16_t)((Wire.read() << 8) | Wire.read());

  // Skip temperature data
  Wire.read(); Wire.read();

  // Read gyroscope data
  data->gyro_x = (int16_t)((Wire.read() << 8) | Wire.read());
  data->gyro_y = (int16_t)((Wire.read() << 8) | Wire.read());
  data->gyro_z = (int16_t)((Wire.read() << 8) | Wire.read());
}

void complementary_filter(MPU6050Data *data, float *angle_x, float *angle_y) {
  static float angle_x_gyro = 0, angle_y_gyro = 0;

  // Integrate the gyroscope data
  angle_x_gyro += data->gyro_x * (SAMPLING_PERIOD_MS / 1000.0f);
  angle_y_gyro += data->gyro_y * (SAMPLING_PERIOD_MS / 1000.0f);

  // Calculate accelerometer angles
  float accel_angle_x = atan2(data->accel_y, data->accel_z) * 180 / PI;
  float accel_angle_y = atan2(data->accel_x, data->accel_z) * 180 / PI;

  // Apply the complementary filter
  *angle_x = COMPLEMENTARY_FILTER_ALPHA * (angle_x_gyro) + (1 - COMPLEMENTARY_FILTER_ALPHA) * accel_angle_x;
  *angle_y = COMPLEMENTARY_FILTER_ALPHA * (angle_y_gyro) + (1 - COMPLEMENTARY_FILTER_ALPHA) * accel_angle_y;
}
