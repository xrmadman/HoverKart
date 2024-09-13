//#include <Arduino.h>
#include "PIDControl.h"
#include "MotorControl.h"
#include "Sensors.h"
#include "BluetoothControl.h"
#include "WiFiControl.h"

// Declare global variables for obstacle detection distances (from WiFiControl.h)
extern int leftUltrasonicDistance;
extern int rightUltrasonicDistance;
extern int rearUltrasonicDistance;
extern int lidarDistance;
int rearUltrasonicDistance = 30;  // Example default value
int lidarDistance = 300;          // Example default value
int leftUltrasonicDistance = 30;  // Example default value
int rightUltrasonicDistance = 30; // Example default value


void setup() {
  // Initialize all systems
  setupSensors();
  setupBluetooth("Hover_Kart");
  setupMotors();
  setupPID();
  setupWiFi();  // Initialize Wi-Fi and start the web server

}

void loop() {
  // Handle Wi-Fi client requests
  handleClientRequests();

  // Check sensor and Bluetooth readings
  int phoneDistance = getPhoneDistance();
  int leftUltrasonic = readUltrasonicDistance(TRIG_PIN_LEFT_ULTRASONIC, ECHO_PIN_LEFT_ULTRASONIC);
  int rightUltrasonic = readUltrasonicDistance(TRIG_PIN_RIGHT_ULTRASONIC, ECHO_PIN_RIGHT_ULTRASONIC);
  int lidar = readLidarDistance();

  // Pathfinding and obstacle avoidance
  handlePathfinding(phoneDistance, leftUltrasonic, rightUltrasonic, lidar);

  delay(100);
}

/**
   Pathfinding logic based on phone distance and ultrasonic/LiDAR sensor readings.
   Adjusts motor speed and direction accordingly.
*/
void handlePathfinding(int phoneDistance, int leftUltrasonic, int rightUltrasonic, int lidar) {
  // If the path is clear (no obstacle detected)
  if ((lidar >= lidarDistance || lidar <= 0) &&
      (leftUltrasonic > leftUltrasonicDistance) &&
      (rightUltrasonic > rightUltrasonicDistance)) {
    // Adjust speed based on phone distance
    double speed = calculateSpeed(phoneDistance);
    setMotorPWM(speed, speed);  // Move forward at the calculated speed
  } else {
    // Obstacle detected, check if we can reverse
    if (isRearPathClear()) {
      performReverse();  // Reverse if the rear path is clear
    } else {
      setMotorPWM(0, 0);  // Stop if unable to reverse
    }
  }
}
