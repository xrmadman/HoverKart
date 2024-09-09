#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <Arduino.h>

// Enum for error types
enum ErrorType {
  SENSOR_ERROR,
  MOTOR_ERROR,
  BLUETOOTH_ERROR,
  LIDAR_ERROR,
  GENERAL_ERROR
};

class ErrorHandler {
  private:
    bool errorOccurred;
    String lastError;
    unsigned long errorTimestamp;
    const unsigned long ERROR_TIMEOUT = 5000;

  public:
    ErrorHandler() : errorOccurred(false), lastError(""), errorTimestamp(0) {}

    // Log and display errors
    void displayError(ErrorType errorType, String message) {
      errorOccurred = true;
      lastError = message;
      errorTimestamp = millis();
      Serial.println("Error: " + message);
      handleRecovery(errorType);
    }

    // Handle recovery actions based on error type
    void handleRecovery(ErrorType errorType) {
      switch (errorType) {
        case MOTOR_ERROR:
          Serial.println("Stopping motors due to error.");
          emergencyStop();
          break;
        default:
          Serial.println("General recovery action.");
          break;
      }
    }

    // Error timeout mechanism
    void handleErrors() {
      if (errorOccurred && millis() - errorTimestamp > ERROR_TIMEOUT) {
        Serial.println("Error timeout exceeded, recovering...");
        errorOccurred = false;  // Reset error state
      }
    }

    // Check if error occurred
    bool hasErrorOccurred() {
      return errorOccurred;
    }
};

// Global instance of the error handler class
ErrorHandler errorHandler;

void emergencyStop() {
  // Stop the motors in an emergency
  Serial.println("Emergency stop triggered.");
}

#endif
