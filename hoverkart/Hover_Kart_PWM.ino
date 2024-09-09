#include "PIDControl.h"
#include "MotorControl.h"
#include "Sensors.h"
#include "BluetoothControl.h"
#include "ErrorHandling.h"
#include "WiFiControl.h"  // Added Wi-Fi control

void setup() {
  Serial.begin(115200);

  // Initialize all systems
  setupBluetooth("Hover_Kart");
  setupMotors();
  setupSensors();
  setupPID();
  setupWiFi();  // Initialize Wi-Fi and start the web server

  Serial.println("Hover Kart setup complete.");
}

void loop() {
  // Handle any errors
  errorHandler.handleErrors();

  // Handle HTTP requests
  handleClientRequests();

  // Example of checking sensor and Bluetooth readings
  int phoneDistance = getPhoneDistance();
  int lidarDistance = readLidarDistance();

  // Implement pathfinding and motion control based on sensor data
  handlePathfinding(phoneDistance, lidarDistance);

  delay(100);  // Adjust delay based on performance needs
}

/**
   Pathfinding logic based on phone distance and LiDAR sensor readings.
   Adjusts motor speed and direction accordingly.
*/
void handlePathfinding(int phoneDistance, int lidarDistance) {
  if (lidarDistance < 300 && lidarDistance > 0) {  // Obstacle detected
    if (isRearPathClear()) {
      performReverse();
    } else {
      setMotorPWM(0, 0);  // Stop if unable to reverse
      errorHandler.displayError(MOTOR_ERROR, "Path blocked, unable to reverse.");
    }
  } else {
    // Adjust speed based on phone distance
    double speed = calculateSpeed(phoneDistance);
    setMotorPWM(speed, speed);  // Move forward at the calculated speed
  }
}
