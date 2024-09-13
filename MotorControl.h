#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "Sensors.h"

#define PWM_PIN_LEFT 32
#define PWM_PIN_RIGHT 33
#define DIRECTION_PIN_LEFT 25
#define DIRECTION_PIN_RIGHT 26

const int DEFAULT_FORWARD_SPEED = 1500;
const int DEFAULT_REVERSE_SPEED = 1000;
const int REVERSE_TIMEOUT = 5000;
extern int rearUltrasonicDistance;
extern int lidarDistance;
extern int leftUltrasonicDistance;
extern int rightUltrasonicDistance;


void setupMotors();
void setMotorPWM(int leftSpeed, int rightSpeed);
void performReverse();
void steerForward();
bool isRearPathClear();

/**
   Sets up the motor control pins.
*/
void setupMotors() {
  pinMode(PWM_PIN_LEFT, OUTPUT);
  pinMode(PWM_PIN_RIGHT, OUTPUT);
  pinMode(DIRECTION_PIN_LEFT, OUTPUT);
  pinMode(DIRECTION_PIN_RIGHT, OUTPUT);
}

/**
   Sets the motor PWM values for left and right motors.
   Speed values are constrained within MIN_SPEED and MAX_SPEED.
*/
void setMotorPWM(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
  analogWrite(PWM_PIN_LEFT, leftSpeed);
  analogWrite(PWM_PIN_RIGHT, rightSpeed);
}

/**
   Perform a reverse operation.
*/
void performReverse() {
  if (isRearPathClear()) {
    digitalWrite(DIRECTION_PIN_LEFT, HIGH);
    digitalWrite(DIRECTION_PIN_RIGHT, HIGH);
    setMotorPWM(DEFAULT_REVERSE_SPEED, DEFAULT_REVERSE_SPEED);
    unsigned long startTime = millis();
    while (millis() - startTime < REVERSE_TIMEOUT) {
      if (!isRearPathClear()) {
        setMotorPWM(0, 0);
        return;
      }
    }
    steerForward();
  } else {
    setMotorPWM(0, 0);
  }
}

/**
   Resets the motors to move forward after reversing.
*/
void steerForward() {
  digitalWrite(DIRECTION_PIN_LEFT, LOW);
  digitalWrite(DIRECTION_PIN_RIGHT, LOW);
}

/**
   Checks if the rear path is clear using the rear ultrasonic sensor.
   @return true if the path is clear, false otherwise.
*/

#endif
