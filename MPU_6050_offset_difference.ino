#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
float gx_sum = 0, gy_sum = 0, gz_sum = 0;
float gx_avg = 0, gy_avg = 0, gz_avg = 0;
float gx_prev_avg = 0, gy_prev_avg = 0, gz_prev_avg = 0;
uint32_t readings_count = 0;

unsigned long start_time;

#define LED_PIN 13
bool blinkState = false;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(38400);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);

    // record the start time
    start_time = millis();
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // accumulate the gyro readings
    gx_sum += gx;
    gy_sum += gy;
    gz_sum += gz;
    readings_count++;

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    // check if 5 minutes have passed
    if (millis() - start_time >= 1 * 60 * 1000) {
        // calculate the average readings
        gx_avg = gx_sum / readings_count;
        gy_avg = gy_sum / readings_count;
        gz_avg = gz_sum / readings_count;

        // subtract previous averages from current averages
        float gx_diff = gx_avg - gx_prev_avg;
        float gy_diff = gy_avg - gy_prev_avg;
        float gz_diff = gz_avg - gz_prev_avg;

        // print the difference with 2 decimal places
        Serial.print("Difference in Gyro Readings: ");
        Serial.print("gx: "); Serial.print(gx_diff, 2);
        Serial.print(", gy: "); Serial.print(gy_diff, 2);
        Serial.print(", gz: "); Serial.println(gz_diff, 2);

        // update previous averages
        gx_prev_avg = gx_avg;
        gy_prev_avg = gy_avg;
        gz_prev_avg = gz_avg;

        // reset sums and count for next 5 minutes
        gx_sum = 0;
        gy_sum = 0;
        gz_sum = 0;
        readings_count = 0;

        // reset start time
        start_time = millis();
    }

    delay(500);
}
