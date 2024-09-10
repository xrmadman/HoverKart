#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include "Sensors.h"       // Include sensor functions to check rear path
#include "ErrorHandling.h" // Include error handling

// Motor control pin definitions
#define PWM_PIN_LEFT 32
#define PWM_PIN_RIGHT 33
#define DIRECTION_PIN_LEFT 25
#define DIRECTION_PIN_RIGHT 26

// Motor speed and timeout configurations
const int DEFAULT_FORWARD_SPEED = 1500;
const int DEFAULT_REVERSE_SPEED = 1000;
const int MAX_SPEED = 2000;
const int MIN_SPEED = 1000;
const int REVERSE_TIMEOUT = 5000;

void setupMotors();
void setMotorPWM(int leftSpeed, int rightSpeed);
void performReverse();
//bool isRearPathClear();
void steerForward();
//void emergencyStop();

/**
   Set up motor control pins as output.
   This function configures the pins for both left and right motors.
*/
void setupMotors() {
  pinMode(PWM_PIN_LEFT, OUTPUT);
  pinMode(PWM_PIN_RIGHT, OUTPUT);
  pinMode(DIRECTION_PIN_LEFT, OUTPUT);
  pinMode(DIRECTION_PIN_RIGHT, OUTPUT);
}

/**
   Set the motor PWM value for both left and right motors.
   The speed values are constrained between MIN_SPEED and MAX_SPEED.
   @param leftSpeed: PWM value for the left motor.
   @param rightSpeed: PWM value for the right motor.
*/
void setMotorPWM(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);  // Constrain left motor speed
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);  // Constrain right motor speed

  analogWrite(PWM_PIN_LEFT, leftSpeed);  // Set PWM for left motor
  analogWrite(PWM_PIN_RIGHT, rightSpeed);  // Set PWM for right motor
}

/**
   Perform a reverse operation, checking the rear ultrasonic sensor before moving.
   If the path is clear, the cart reverses for a limited time.
   If the path is blocked, it stops and reports an error.
*/
void performReverse() {
  if (isRearPathClear()) {  // Check if the rear path is clear
    digitalWrite(DIRECTION_PIN_LEFT, HIGH);  // Set motor to reverse direction
    digitalWrite(DIRECTION_PIN_RIGHT, HIGH);  // Set motor to reverse direction
    setMotorPWM(DEFAULT_REVERSE_SPEED, DEFAULT_REVERSE_SPEED);  // Set reverse speed

    // Reverse for a limited time, unless the path becomes blocked
    unsigned long startTime = millis();
    while (millis() - startTime < REVERSE_TIMEOUT) {
      if (!isRearPathClear()) {  // Stop reversing if the path is blocked
        emergencyStop();
        return;
      }
    }
    steerForward();  // After timeout, return to forward direction
  } else {
   errorHandler.displayError(ErrorType::MOTOR_ERROR, "Rear path blocked, cannot reverse.");

  }
}

/**
   Check if the rear path is clear using the rear ultrasonic sensor.
   @return: True if the rear path is clear, otherwise false.
*/

/**
   Steer both motors back to the forward direction after reversing.
   This function is called after a successful reverse or when resuming normal operation.
*/
void steerForward() {
  digitalWrite(DIRECTION_PIN_LEFT, LOW);  // Set motor to forward direction
  digitalWrite(DIRECTION_PIN_RIGHT, LOW);  // Set motor to forward direction
}

/**
   Stop both motors immediately in case of an emergency.
   This function is called by the error handler during a motor error.
*/
void emergencyStop() {
  setMotorPWM(0, 0);  // Stop both motors
  Serial.println("Emergency stop triggered.");
}

#endif
