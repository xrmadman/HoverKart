#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>

// Pin definitions for ultrasonic sensors
#define TRIG_PIN_LEFT_ULTRASONIC 13
#define ECHO_PIN_LEFT_ULTRASONIC 12
#define TRIG_PIN_RIGHT_ULTRASONIC 14
#define ECHO_PIN_RIGHT_ULTRASONIC 27
#define TRIG_PIN_REAR_ULTRASONIC 15
#define ECHO_PIN_REAR_ULTRASONIC 26

// LiDAR I2C address and distance limits
const int LIDAR_ADDRESS = 0x62;
const int LIDAR_MAX_DISTANCE = 1200;
const int MIN_SAFE_DISTANCE = 30;


// Ultrasonic sensor configuration
extern int leftUltrasonicDistance;  // From WiFiControl.h
extern int rightUltrasonicDistance;
extern int rearUltrasonicDistance;
extern int lidarDistance;

void setupSensors();
int readUltrasonicDistance(int trigPin, int echoPin);
int readLidarDistance();
bool isRearPathClear();
const int MAX_ULTRASONIC_DISTANCE = 400;

/**
   Set up sensors including ultrasonic sensors and LiDAR.
*/
void setupSensors() {
  Wire.begin();

  // Set up ultrasonic sensor pins
  pinMode(TRIG_PIN_LEFT_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_LEFT_ULTRASONIC, INPUT);
  pinMode(TRIG_PIN_RIGHT_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_RIGHT_ULTRASONIC, INPUT);
  pinMode(TRIG_PIN_REAR_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_REAR_ULTRASONIC, INPUT);
}

/**
   Reads the distance from an ultrasonic sensor.
*/
int readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // Max timeout
  if (duration == 0) {
    return -1;  // Sensor timeout
  }

  int distance = duration * 0.034 / 2;
  return constrain(distance, 0, MAX_ULTRASONIC_DISTANCE);
}

/**
   Reads the distance from the rear ultrasonic sensor.
*/
bool isRearPathClear() {
  int distanceBack = readUltrasonicDistance(TRIG_PIN_REAR_ULTRASONIC, ECHO_PIN_REAR_ULTRASONIC);
  return distanceBack > rearUltrasonicDistance;
}

/**
   Reads the distance from the LiDAR sensor via I2C.
*/
int readLidarDistance() {
  Wire.beginTransmission(LIDAR_ADDRESS);
  Wire.write(0x00);  // Command to initiate measurement
  Wire.endTransmission();

  Wire.requestFrom(LIDAR_ADDRESS, 2);
  if (Wire.available() == 2) {
    int distance = Wire.read() << 8 | Wire.read();
    if (distance <= 0 || distance > LIDAR_MAX_DISTANCE) {
      return -1;
    }
    return distance;
  }
  return -1;  // Error reading LiDAR
}

#endif
