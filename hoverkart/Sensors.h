#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>

#define TRIG_PIN_FRONT_ULTRASONIC 13
#define ECHO_PIN_FRONT_ULTRASONIC 12
#define TRIG_PIN_REAR_ULTRASONIC 14
#define ECHO_PIN_REAR_ULTRASONIC 27

const int MAX_ULTRASONIC_DISTANCE = 400;
const int MIN_SAFE_DISTANCE = 30;
const int LIDAR_ADDRESS = 0x62;
const int LIDAR_MAX_DISTANCE = 1200;

void setupSensors();
int readFrontUltrasonic();
bool isRearPathClear();
int readLidarDistance();

void setupSensors() {
  Wire.begin();
  pinMode(TRIG_PIN_FRONT_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_FRONT_ULTRASONIC, INPUT);
  pinMode(TRIG_PIN_REAR_ULTRASONIC, OUTPUT);
  pinMode(ECHO_PIN_REAR_ULTRASONIC, INPUT);

  Wire.beginTransmission(LIDAR_ADDRESS);
  if (Wire.endTransmission() != 0) {
    errorHandler.displayError(SENSOR_ERROR, "LiDAR initialization failed.");
  }
}

int readFrontUltrasonic() {
  digitalWrite(TRIG_PIN_FRONT_ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_FRONT_ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_FRONT_ULTRASONIC, LOW);

  long duration = pulseIn(ECHO_PIN_FRONT_ULTRASONIC, HIGH, MAX_ULTRASONIC_DISTANCE * 58);

  if (duration == 0) {
    errorHandler.displayError(SENSOR_ERROR, "Front ultrasonic sensor timeout.");
    return -1;
  }

  int distance = duration * 0.034 / 2;
  return constrain(distance, 0, MAX_ULTRASONIC_DISTANCE);
}

bool isRearPathClear() {
  return readFrontUltrasonic() > MIN_SAFE_DISTANCE;
}

int readLidarDistance() {
  Wire.beginTransmission(LIDAR_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(LIDAR_ADDRESS, 2);
  if (Wire.available() == 2) {
    int distance = Wire.read() << 8 | Wire.read();
    if (distance <= 0 || distance > LIDAR_MAX_DISTANCE) {
      errorHandler.displayError(SENSOR_ERROR, "LiDAR invalid distance.");
      return -1;
    }
    return distance;
  }
  errorHandler.displayError(SENSOR_ERROR, "LiDAR read failed.");
  return -1;
}

#endif
