#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#define PWM_PIN_LEFT 32
#define PWM_PIN_RIGHT 33
#define DIRECTION_PIN_LEFT 25
#define DIRECTION_PIN_RIGHT 26

const int DEFAULT_FORWARD_SPEED = 1500;
const int DEFAULT_REVERSE_SPEED = 1000;
const int MAX_SPEED = 2000;
const int MIN_SPEED = 1000;
const int REVERSE_TIMEOUT = 5000;

void setupMotors();
void setMotorPWM(int leftSpeed, int rightSpeed);
void performReverse();
bool isRearPathClear();
void steerForward();
void emergencyStop();

void setupMotors() {
  pinMode(PWM_PIN_LEFT, OUTPUT);
  pinMode(PWM_PIN_RIGHT, OUTPUT);
  pinMode(DIRECTION_PIN_LEFT, OUTPUT);
  pinMode(DIRECTION_PIN_RIGHT, OUTPUT);
}

void setMotorPWM(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
  analogWrite(PWM_PIN_LEFT, leftSpeed);
  analogWrite(PWM_PIN_RIGHT, rightSpeed);
}

void performReverse() {
  if (isRearPathClear()) {
    digitalWrite(DIRECTION_PIN_LEFT, HIGH);
    digitalWrite(DIRECTION_PIN_RIGHT, HIGH);
    setMotorPWM(DEFAULT_REVERSE_SPEED, DEFAULT_REVERSE_SPEED);

    unsigned long startTime = millis();
    while (millis() - startTime < REVERSE_TIMEOUT) {
      if (!isRearPathClear()) {
        emergencyStop();
        return;
      }
    }
    steerForward();
  } else {
    errorHandler.displayError(MOTOR_ERROR, "Rear path blocked.");
  }
}

bool isRearPathClear() {
  int distanceBack = readUltrasonicDistance(TRIG_PIN_REAR_ULTRASONIC, ECHO_PIN_REAR_ULTRASONIC);
  return distanceBack > 30;
}

void steerForward() {
  digitalWrite(DIRECTION_PIN_LEFT, LOW);
  digitalWrite(DIRECTION_PIN_RIGHT, LOW);
}

#endif
