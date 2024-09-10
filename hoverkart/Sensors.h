#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "ErrorHandling.h"  // Ensure error handling is included

// Pin definitions for left, right, and rear ultrasonic sensors
#define TRIG_PIN_LEFT_ULTRASONIC 13
#define ECHO_PIN_LEFT_ULTRASONIC 12
#define TRIG_PIN_RIGHT_ULTRASONIC 14
#define ECHO_PIN_RIGHT_ULTRASONIC 27
#define TRIG_PIN_REAR_ULTRASONIC 15
#define ECHO_PIN_REAR_ULTRASONIC 26

// LiDAR I2C address and distance limits
const int LIDAR_ADDRESS = 0x62;
const int LIDAR_MAX_DISTANCE = 1200;

// Ultrasonic sensor configuration
const int MAX_ULTRASONIC_DISTANCE = 400;  // Maximum distance for ultrasonic readings
const int MIN_SAFE_DISTANCE = 30;  // Minimum safe distance to avoid obstacles

void setupSensors();
int readUltrasonicDistance(int trigPin, int echoPin);
int readLidarDistance();
bool isRearPathClear();

/**
   Set up sensors including ultrasonic sensors and LiDAR.
   Initializes I2C communication for the LiDAR sensor and configures the pin modes.
*/
void setupSensors() {
  // Initialize I2C communication for LiDAR
  Wire.begin();

  // Set up the ultrasonic sensor pins
  pinMode(TRIG_PIN_LEFT_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_LEFT_ULTRASONIC, INPUT);
  pinMode(TRIG_PIN_RIGHT_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_RIGHT_ULTRASONIC, INPUT);
  pinMode(TRIG_PIN_REAR_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_REAR_ULTRASONIC, INPUT);

  // Verify LiDAR initialization
  Wire.beginTransmission(LIDAR_ADDRESS);
  if (Wire.endTransmission() != 0) {
    errorHandler.displayError(ErrorType::SENSOR_ERROR, "LiDAR initialization failed.");

  }
}

/**
   Reads the distance from an ultrasonic sensor.
   @param trigPin: Trigger pin for the ultrasonic sensor.
   @param echoPin: Echo pin for the ultrasonic sensor.
   @return: Distance in centimeters, or -1 if there is an error.
*/
int readUltrasonicDistance(int trigPin, int echoPin) {
  // Send a pulse to the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the time of the pulse's return
  long duration = pulseIn(echoPin, HIGH, MAX_ULTRASONIC_DISTANCE * 58);

  // Check for timeout
  if (duration == 0) {
    errorHandler.displayError(ErrorType::SENSOR_ERROR, "Ultrasonic sensor timeout.");
    return -1;
  }

  // Calculate distance in centimeters
  int distance = duration * 0.034 / 2;
  return constrain(distance, 0, MAX_ULTRASONIC_DISTANCE);
}

/**
   Reads the distance from the rear ultrasonic sensor to determine if reversing is safe.
   @return: True if the rear path is clear, otherwise false.
*/
bool isRearPathClear() {
  int distanceBack = readUltrasonicDistance(TRIG_PIN_REAR_ULTRASONIC, ECHO_PIN_REAR_ULTRASONIC);
  return distanceBack > MIN_SAFE_DISTANCE;
}

/**
   Reads the distance from the LiDAR sensor via I2C.
   @return: Distance in centimeters, or -1 if there is an error.
*/
int readLidarDistance() {
  // Request LiDAR measurement data
  Wire.beginTransmission(LIDAR_ADDRESS);
  Wire.write(0x00);  // Command to initiate measurement
  Wire.endTransmission();

  // Read 2 bytes from the LiDAR sensor
  Wire.requestFrom(LIDAR_ADDRESS, 2);
  if (Wire.available() == 2) {
    int distance = Wire.read() << 8 | Wire.read();  // Combine two bytes into one distance value
    if (distance <= 0 || distance > LIDAR_MAX_DISTANCE) {
      errorHandler.displayError(ErrorType::SENSOR_ERROR, "LiDAR invalid distance.");
      return -1;
    }
    return distance;
  }

  // Handle failure to read from LiDAR
  errorHandler.displayError(ErrorType::SENSOR_ERROR, "LiDAR read failed.");
  return -1;
}

#endif
