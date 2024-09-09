#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <PID_v1.h>

double Kp = 2.5, Ki = 6.0, Kd = 1.2;
double setpointLeft, inputLeft, outputLeft;
double setpointRight, inputRight, outputRight;
PID leftWheelPID(&inputLeft, &outputLeft, &setpointLeft, Kp, Ki, Kd, DIRECT);
PID rightWheelPID(&inputRight, &outputRight, &setpointRight, Kp, Ki, Kd, DIRECT);

const int MIN_SPEED = 1000;
const int MAX_SPEED = 2000;

void setupPID() {
  leftWheelPID.SetMode(AUTOMATIC);
  rightWheelPID.SetMode(AUTOMATIC);
  leftWheelPID.SetOutputLimits(MIN_SPEED, MAX_SPEED);
  rightWheelPID.SetOutputLimits(MIN_SPEED, MAX_SPEED);
}

double calculateSpeed(int distanceToPhone) {
  if (distanceToPhone < 0) {
    errorHandler.displayError(GENERAL_ERROR, "Invalid distance to phone.");
    return MIN_SPEED;
  }

  double speed = map(distanceToPhone, 100, 300, MIN_SPEED, MAX_SPEED);
  return constrain(speed, MIN_SPEED, MAX_SPEED);
}

#endif
