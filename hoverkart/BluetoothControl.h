#ifndef BLUETOOTHCONTROL_H
#define BLUETOOTHCONTROL_H

#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
const char* DEFAULT_BLUETOOTH_NAME = "Hover_Kart";

void setupBluetooth(const char* btName = DEFAULT_BLUETOOTH_NAME) {
  if (!SerialBT.begin(btName)) {
  }
}
int getPhoneDistance() {
  if (!SerialBT.hasClient()) {

    return -1;
  }
  int distance = random(100, 500);  // Simulated distance for testing
  if (distance < 0 || distance > 1000) {
    return -1;
  }
  return distance;
}
#endif
