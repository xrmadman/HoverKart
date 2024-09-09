#ifndef BLUETOOTHCONTROL_H
#define BLUETOOTHCONTROL_H

#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
const char* DEFAULT_BLUETOOTH_NAME = "Hover_Kart";

void setupBluetooth(const char* btName = DEFAULT_BLUETOOTH_NAME) {
  if (!SerialBT.begin(btName)) {
    errorHandler.displayError(BLUETOOTH_ERROR, "Bluetooth initialization failed.");
  } else {
    Serial.println("Bluetooth started with name: " + String(btName));
  }
}

int getPhoneDistance() {
  if (!SerialBT.hasClient()) {
    errorHandler.displayError(BLUETOOTH_ERROR, "Bluetooth not connected.");
    return -1;
  }

  int distance = random(100, 500);
  if (distance < 0 || distance > 1000) {
    errorHandler.displayError(BLUETOOTH_ERROR, "Invalid distance value.");
    return -1;
  }

  return distance;
}

#endif
