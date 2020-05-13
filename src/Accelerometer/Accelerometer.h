#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Adafruit_LIS3DH.h>

/**
* Tracks and returns the acceleration detected by the accelerometer.
*   
* By Julia Behnen
* 
* Adapted from the acceldemo example code referenced at https://learn.adafruit.com/adafruit-lis3dh-triple-axis-accelerometer-breakout/arduino
*/ 
class Accelerometer {
  private:
    Adafruit_LIS3DH _lis; // Accelerometer
    int _accelerationMagnitude; // Most recent measurement
    
  public:
    // Constructor
    Accelerometer();
	
    /*
     * Sets up the class. Must be called before any other methods.
     */
    void setup(); 
    
    /*
     * Updates the state of the class based on the accelerometer. 
     */
    void update();
    
    /*
     * Returns acceleration magnitude detected and calculated
     * in update().
     */
    int getAccelerationMagnitude();
};

#endif