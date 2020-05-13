#include "Accelerometer.h"
#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

// Adapted from the acceldemo example code referenced at https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/arduino

Accelerometer :: Accelerometer() {
  _lis = Adafruit_LIS3DH();
}

void Accelerometer :: setup() {
  Serial.println("LIS3DH test!");

  if (! _lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  _lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << _lis.getRange());
  Serial.println("G");

  _lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  Serial.print("Data rate set to: ");
  switch (_lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;

    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
  }
 
}

void Accelerometer :: update() {
  sensors_event_t event;
  _lis.getEvent(&event);
  
  _accelerationMagnitude = floor(sqrt(
      pow(event.acceleration.x, 2) 
    + pow(event.acceleration.y, 2) 
    + pow(event.acceleration.z, 2)));
}

int Accelerometer :: getAccelerationMagnitude() {
  return _accelerationMagnitude;
}