#include <Wire.h>

#define HMC5883_ADDR 0x1E // I2C address for the HMC5883 magnetometer

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize HMC5883
  hmc5883_init();
}

void loop() {
  // Read magnetometer data
  int16_t x, y, z;
  hmc5883_read(&x, &y, &z);

  // Print magnetometer data
  Serial.print("Magnetometer Data (X, Y, Z): ");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z);

  delay(1000); // Delay for readability
}

void hmc5883_init() {
  Wire.beginTransmission(HMC5883_ADDR);
  Wire.write(0x02); // Mode register
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission();
}

void hmc5883_read(int16_t *x, int16_t *y, int16_t *z) {
  Wire.beginTransmission(HMC5883_ADDR);
  Wire.write(0x03); // Data output X MSB register
  Wire.endTransmission(false);
  Wire.requestFrom(HMC5883_ADDR, (uint8_t)6);

  if (Wire.available() >= 6) {
    *x = Wire.read() << 8 | Wire.read();
    *z = Wire.read() << 8 | Wire.read();
    *y = Wire.read() << 8 | Wire.read();
  }
}
