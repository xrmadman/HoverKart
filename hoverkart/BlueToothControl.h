#ifndef BLUETOOTHCONTROL_H
#define BLUETOOTHCONTROL_H

#include <BluetoothSerial.h>
#include "ErrorHandling.h"  // Include error handling

// Initialize Bluetooth Serial
BluetoothSerial SerialBT;
const char* DEFAULT_BLUETOOTH_NAME = "Hover_Kart";  // Default Bluetooth name

/**
   Set up Bluetooth connection with the specified name.
   If initialization fails, logs the error using the error handler.
   @param btName: The name of the Bluetooth device (optional).
*/
void setupBluetooth(const char* btName = DEFAULT_BLUETOOTH_NAME) {
  if (!SerialBT.begin(btName)) {  // Try to initialize Bluetooth
    errorHandler.displayError(ErrorType::BLUETOOTH_ERROR, "Bluetooth initialization failed.");
  } else {
    Serial.println("Bluetooth started with name: " + String(btName));
  }
}

/**
   Simulate or retrieve the distance to the phone via Bluetooth.
   If Bluetooth is disconnected, returns an error.
   @return: Simulated phone distance in cm (or -1 if disconnected or invalid).
*/
int getPhoneDistance() {
  if (!SerialBT.hasClient()) {  // Check if Bluetooth client is connected
    errorHandler.displayError(ErrorType::BLUETOOTH_ERROR, "Bluetooth not connected.");
    return -1;
  }

  int distance = random(100, 500);  // Simulate distance data for now
  if (distance < 0 || distance > 1000) {  // Validate distance range
    errorHandler.displayError(ErrorType::BLUETOOTH_ERROR, "Invalid distance value.");
    return -1;
  }

  return distance;  // Return valid distance
}

#endif
