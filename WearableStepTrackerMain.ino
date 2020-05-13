/**
 * Runs a wearable step tracker with an accelerometer and a 
 * 128 x 64 OLED display.
 * 
 * This code assumes a fixed placement of the device and was
 * primarily tested with a wrist placement.
 * 
 * By Julia Behnen
 */

#include "src/Accelerometer/Accelerometer.h"
#include "src/Oled/Oled.h"
#include "src/StepCounter/StepCounter.h"

// Credit to OLED citation in Oled.h
const int SCREEN_WIDTH = 128; // px
const int SCREEN_HEIGHT = 64; // px
const int OLED_RESET_PIN = 4; // Reset pin # (or -1 if sharing Arduino reset pin)

const int DELAY_MS = 200;

// https://github.com/makeabilitylab/arduino/blob/master/ESP32/BatteryLevel/BatteryLevel.ino
const float MAX_ANALOG_VAL = 4095.0;
const float MAX_BATTERY_VOLTAGE  = 4.2; // Volts

// Credit to step counter citation in StepCounter.h
const int MIN_DISTANCE_BETWEEN_PEAKS_MS = 300;
const int MIN_PEAK_HEIGHT = 2;
const int SMOOTHING_WINDOW_SIZE = 5;

const int VOLTAGE_PIN = A13; // voltage needs to be multiplied by 2
const int TREAT_BUTTON_PIN = 21;

const int SAMPLING_RATE = 50; // Hz
const int SAMPLING_INTERVAL = 
  floor(1000/((float) SAMPLING_RATE)); // Ms

const int REFRESH_INTERVAL = 200; // Ms interval for OLED refresh

const int JUMP_HEIGHT = -10; // pixels

const int STEPS_PER_BONE = 5;

int _steps; // # total steps calculated by the accelerometer
int _displaySteps; // # steps displayed on the OLED

// Credit to accelerometer citation in Accelerometer.h
int _accelerationMagnitude;
long _accelerationTimestamp;

long _lastRefreshTimestamp;

int _totalTreats;
int _spentTreats;

// Credit to accelerometer citation in Accelerometer.h
Accelerometer accelerometer = Accelerometer();
    
// Credit to OLED citation in Oled.h
Oled oled = Oled(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_RESET_PIN);

// Partially derived from step calcuator citation in StepCalculator.h
StepCounter stepCounter = 
  StepCounter(MIN_DISTANCE_BETWEEN_PEAKS_MS, 
              MIN_PEAK_HEIGHT, 
              SAMPLING_RATE,
              SMOOTHING_WINDOW_SIZE);

void setup(){
  Serial.begin(115200);
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(SCL, INPUT_PULLUP);
  pinMode(SDA, INPUT_PULLUP);
  pinMode(TREAT_BUTTON_PIN, INPUT_PULLUP);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  accelerometer.setup();
  oled.setup();
  long timestamp = millis();
  _accelerationTimestamp = timestamp - SAMPLING_INTERVAL;
  _lastRefreshTimestamp = timestamp - REFRESH_INTERVAL;
  _displaySteps = 0;
}

void loop(){
  long timestamp = millis();
  if (timestamp >= _accelerationTimestamp + SAMPLING_INTERVAL) {
    updateAccelerometer();
    updateStepCounter();
    _accelerationTimestamp = timestamp;
  }
  if (timestamp >= _lastRefreshTimestamp + REFRESH_INTERVAL) {
    updateOled();
    _lastRefreshTimestamp = timestamp;
  }
}

/*
 * Updates the accelerometer class and updates the acceleration magnitude 
 * stored in this class to the acceleration magnitude calculated by 
 * accelerometer class.
 */
void updateAccelerometer() {
  accelerometer.update();
  _accelerationTimestamp = millis();
  _accelerationMagnitude = accelerometer.getAccelerationMagnitude();
}

/*
 * Updates the step counter and updates the number of steps stored in
 * this class to the number of steps calculated by the step counter.
 */
void updateStepCounter() {
  stepCounter.update(_accelerationMagnitude, _accelerationTimestamp);
  _steps = stepCounter.getSteps();
}

/*
 * Updates the OLED display as well as the status of the treats.
 */
void updateOled() {
  int currentJumpHeight = 0;
  if (_steps > _displaySteps) {
    _displaySteps = _steps;
    currentJumpHeight = JUMP_HEIGHT;
  }

  _totalTreats  = floor(_displaySteps / STEPS_PER_BONE);
  int remainingBones = _totalTreats - _spentTreats;
  bool giveTreat = false;
  if (isButtonPressed(TREAT_BUTTON_PIN) && remainingBones > 0) {
    currentJumpHeight = JUMP_HEIGHT;
    _spentTreats += 1;
    giveTreat = true;
  } 

  oled.update(currentJumpHeight, 
              _displaySteps, 
              _totalTreats, 
              remainingBones, 
              batteryPercentage(),
              giveTreat);  
}

/*
 * Returns the battery percentage rounded down to the nearest integer.
 * 
 * Adapted from https://github.com/makeabilitylab/arduino/blob/master/ESP32/BatteryLevel/BatteryLevel.ino
 */
int batteryPercentage() {
  int inputReading = analogRead(VOLTAGE_PIN);
  float voltageLevel = 2 * 1.1 * 3.3 * inputReading / MAX_ANALOG_VAL;
  float percentage = 100 * voltageLevel / MAX_BATTERY_VOLTAGE;
  return floor(percentage);
}

/*
 * Returns true if the button at btnPin is pressed, false otherwise.
 * 
 * Assumes a pull-up resistor configuration.
 */
// Lightly adapted from https://makeabilitylab.github.io/physcomp/arduino/piano.html
boolean isButtonPressed(int btnPin){
  return digitalRead(btnPin) == LOW;
}
