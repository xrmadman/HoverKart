#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <Arduino.h>
#include "MotorControl.h"  // Include motor control for stopping motors


// Enum for error types, including Wi-Fi
enum ErrorType {
  SENSOR_ERROR,     // Error with sensors (e.g., ultrasonic, LiDAR)
  MOTOR_ERROR,      // Error with motors (e.g., stalled, blocked path)
  BLUETOOTH_ERROR,  // Error with Bluetooth communication
  LIDAR_ERROR,      // Error with LiDAR sensor
  WIFI_ERROR,       // Error with Wi-Fi connection or setup
  GENERAL_ERROR     // General system errors (e.g., invalid inputs)
};

/**
   Class to handle errors and perform recovery actions.
*/
class ErrorHandler {
  private:
    bool errorOccurred;  // Flag indicating if an error has occurred
    String lastError;    // Stores the last error message
    unsigned long errorTimestamp;  // Timestamp when the error occurred
    const unsigned long ERROR_TIMEOUT = 5000;  // Timeout duration for error handling

  public:
    // Constructor initializes error states
    ErrorHandler() : errorOccurred(false), lastError(""), errorTimestamp(0) {}

    /**
       Display and log an error message.
       @param errorType: Type of the error (e.g., MOTOR_ERROR, SENSOR_ERROR).
       @param message: Description of the error.
    */
    void displayError(ErrorType errorType, String message) {
      errorOccurred = true;  // Mark that an error occurred
      lastError = message;   // Store the last error message
      errorTimestamp = millis();  // Record the time of the error
      Serial.println("Error: " + message);  // Print the error message to the serial console
      handleRecovery(errorType);  // Call the recovery handler based on the error type
    }

    /**
       Perform recovery actions based on the type of error.
       Different errors may require different recovery actions.
       @param errorType: Type of the error that occurred.
    */
    void handleRecovery(ErrorType errorType) {
      switch (errorType) {
        case MOTOR_ERROR:
          Serial.println("Stopping motors due to error.");
          emergencyStop();  // Call emergency stop when motor error occurs
          break;
        case WIFI_ERROR:
          Serial.println("Wi-Fi error, attempting recovery.");
          // Implement Wi-Fi recovery logic here if needed
          break;
        default:
          Serial.println("General recovery action.");
          break;
      }
    }

    /**
       Handle errors with a timeout mechanism.
       Resets the error state after a set period of time.
    */
    void handleErrors() {
      if (errorOccurred && millis() - errorTimestamp > ERROR_TIMEOUT) {
        Serial.println("Error timeout exceeded, recovering...");
        errorOccurred = false;  // Reset error state after timeout
      }
    }

    /**
       Check if an error has occurred.
       @return: True if an error is active, otherwise false.
    */
    bool hasErrorOccurred() {
      return errorOccurred;
    }
};

// Global instance of the ErrorHandler class for easy access throughout the code
extern ErrorHandler errorHandler;

/**
   Emergency stop function to halt motors in critical situations.
   Stops the motors by setting the PWM values to 0 for both.
*/


#endif
