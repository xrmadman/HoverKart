#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <PID_v1.h>

// PID tuning parameters (Kp, Ki, Kd) for motor control
double Kp = 2.5, Ki = 6.0, Kd = 1.2;
double setpointLeft, inputLeft, outputLeft;
double setpointRight, inputRight, outputRight;

// Initialize PID objects for both motors (left and right)
PID leftWheelPID(&inputLeft, &outputLeft, &setpointLeft, Kp, Ki, Kd, DIRECT);
PID rightWheelPID(&inputRight, &outputRight, &setpointRight, Kp, Ki, Kd, DIRECT);

// Speed constraints for motor control
const int MIN_SPEED = 1000;
const int MAX_SPEED = 2000;

/**
   Set up the PID control for both motors, with automatic mode and speed limits.
*/
void setupPID() {
  leftWheelPID.SetMode(AUTOMATIC);  // Set left motor PID to automatic mode
  rightWheelPID.SetMode(AUTOMATIC);  // Set right motor PID to automatic mode
  leftWheelPID.SetOutputLimits(MIN_SPEED, MAX_SPEED);  // Limit output for the left motor
  rightWheelPID.SetOutputLimits(MIN_SPEED, MAX_SPEED);  // Limit output for the right motor
}

/**
   Calculate motor speed based on the distance to the phone.
   Uses a mapped value to adjust speed within safe motor limits.
   @param distanceToPhone: Distance from the phone in cm.
   @return Calculated motor speed constrained between MIN_SPEED and MAX_SPEED.
*/
double calculateSpeed(int distanceToPhone) {
  // Ensure the input distance is valid
  if (distanceToPhone < 0) {
    return MIN_SPEED;  // Return minimum speed in case of invalid input
  }

  // Map the phone distance to the motor speed range
  double speed = map(distanceToPhone, 100, 300, MIN_SPEED, MAX_SPEED);
  return constrain(speed, MIN_SPEED, MAX_SPEED);  // Ensure speed is within limits
}

#endif
