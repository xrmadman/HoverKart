#include <Arduino.h>
#include "PIDControl.h"
#include "MotorControl.h"
#include "Sensors.h"
#include "BluetoothControl.h"
#include "ErrorHandling.h"
#include "WiFiControl.h"  // Wi-Fi control

// Forward declaration of the handlePathfinding function
void handlePathfinding(int phoneDistance, int lidarDistance);

// Define the global instance of ErrorHandler
ErrorHandler errorHandler;

void setup() {
  Serial.begin(115200);

  // Initialize all systems
  setupSensors();  // Initialize sensors first
  setupBluetooth("Hover_Kart");  // Initialize Bluetooth
  setupMotors();  // Initialize motor controls
  setupPID();  // Initialize PID control
  setupWiFi();  // Initialize Wi-Fi and start the web server

  Serial.println("Hover Kart setup complete.");
}

void loop() {
  // Handle any errors
  errorHandler.handleErrors();

  // Handle HTTP requests (for Wi-Fi settings)
  handleClientRequests();

  // Check distance from phone (Bluetooth) and obstacles (LiDAR)
  int phoneDistance = getPhoneDistance();
  int lidarDistance = readLidarDistance();

  // Execute pathfinding and motor control based on sensor data
  handlePathfinding(phoneDistance, lidarDistance);

  // Add a small delay for stability
  delay(100);  // Adjust delay based on performance needs
}

/**
   Pathfinding logic based on phone distance and LiDAR sensor readings.
   Adjusts motor speed and direction accordingly.
   @param phoneDistance: Distance from the phone in cm (via Bluetooth).
   @param lidarDistance: Distance from the obstacle in cm (via LiDAR).
*/
void handlePathfinding(int phoneDistance, int lidarDistance) {
  if (lidarDistance >= 300 || lidarDistance <= 0) {  // No obstacle detected
    // Adjust speed based on the distance to the phone
    double speed = calculateSpeed(phoneDistance);
    setMotorPWM(speed, speed);  // Move forward at the calculated speed
  } else {  // Obstacle detected
    if (isRearPathClear()) {
      performReverse();  // Reverse if the path behind is clear
    } else {
      setMotorPWM(0, 0);  // Stop if unable to reverse
      errorHandler.displayError(ErrorType::MOTOR_ERROR, "Path blocked, unable to reverse.");
    }
  }
}
