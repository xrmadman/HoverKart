#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// Wi-Fi credentials and settings
const char* ssid = "HoverKart";
const char* password = "kartpassword";
const char* hostname = "hoverkart";

// Web server object
WebServer server(80);

// PID tuning parameters and following distance variables
double webKp = 2.5, webKi = 6.0, webKd = 1.2;
int followingDistance = 100;
int waitTime = 0;

// Obstacle detection distances (modifiable via web interface)
int ultrasonicLeftDistance = 300;
int ultrasonicRightDistance = 300;
int ultrasonicRearDistance = 300;
int lidarDistanceThreshold = 1000;

// Function prototypes
void setupWiFi();
void handleRoot();
void handleUpdatePID();
void handleUpdateDistance();
void handleUpdateObstacleDetection();
void handleWaitMode();

/**
   Setup Wi-Fi in AP mode and initialize the web server.
*/
void setupWiFi() {
  WiFi.softAP(ssid, password);
  WiFi.softAPsetHostname(hostname);

  if (MDNS.begin(hostname)) {
    MDNS.addService("http", "tcp", 80);
  } else {
  }

  server.on("/", handleRoot);
  server.on("/updatePID", handleUpdatePID);
  server.on("/updateDistance", handleUpdateDistance);
  server.on("/updateObstacleDetection", handleUpdateObstacleDetection);
  server.on("/waitMode", handleWaitMode);

  server.begin();
}

/**
   Main control page that allows adjusting PID values, following distance, obstacle detection distances, and wait time.
*/
void handleRoot() {
  String page = "<html><head><title>Kart Control</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<style>body {font-family: Arial; padding: 20px;} ";
  page += "input[type=number], input[type=submit] {width: 100%; padding: 10px; margin: 10px 0; box-sizing: border-box;}";
  page += "@media (min-width: 600px) {input[type=number], input[type=submit] {width: 50%;}}</style></head><body>";

  page += "<h1>Hover Kart Settings</h1>";

  // PID tuning form
  page += "<form action='/updatePID' method='POST'>";
  page += "Kp: <input type='number' name='kp' step='0.1' value='" + String(webKp) + "'><br>";
  page += "Ki: <input type='number' name='ki' step='0.1' value='" + String(webKi) + "'><br>";
  page += "Kd: <input type='number' name='kd' step='0.1' value='" + String(webKd) + "'><br>";
  page += "<input type='submit' value='Update PID'></form><br>";

  // Following distance form
  page += "<form action='/updateDistance' method='POST'>";
  page += "Following Distance (cm): <input type='number' name='distance' value='" + String(followingDistance) + "'><br>";
  page += "<input type='submit' value='Update Distance'></form><br>";

  // Obstacle detection distance form
  page += "<form action='/updateObstacleDetection' method='POST'>";
  page += "Left Ultrasonic Distance (cm): <input type='number' name='leftDist' value='" + String(ultrasonicLeftDistance) + "'><br>";
  page += "Right Ultrasonic Distance (cm): <input type='number' name='rightDist' value='" + String(ultrasonicRightDistance) + "'><br>";
  page += "Rear Ultrasonic Distance (cm): <input type='number' name='rearDist' value='" + String(ultrasonicRearDistance) + "'><br>";
  page += "LiDAR Distance (cm): <input type='number' name='lidarDist' value='" + String(lidarDistanceThreshold) + "'><br>";
  page += "<input type='submit' value='Update Obstacle Detection'></form><br>";

  // Wait mode form
  page += "<form action='/waitMode' method='POST'>";
  page += "Wait Time (seconds): <input type='number' name='wait' value='" + String(waitTime) + "'><br>";
  page += "<input type='submit' value='Set Wait Mode'></form><br>";

  page += "</body></html>";

  server.send(200, "text/html", page);
}

/**
   Handle form submission to update PID values.
*/
void handleUpdatePID() {
  if (server.hasArg("kp")) webKp = server.arg("kp").toDouble();
  if (server.hasArg("ki")) webKi = server.arg("ki").toDouble();
  if (server.hasArg("kd")) webKd = server.arg("kd").toDouble();

  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle form submission to update following distance.
*/
void handleUpdateDistance() {
  if (server.hasArg("distance")) followingDistance = server.arg("distance").toInt();

  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle form submission to update obstacle detection distances for the ultrasonic sensors and LiDAR.
*/
void handleUpdateObstacleDetection() {
  if (server.hasArg("leftDist")) ultrasonicLeftDistance = server.arg("leftDist").toInt();
  if (server.hasArg("rightDist")) ultrasonicRightDistance = server.arg("rightDist").toInt();
  if (server.hasArg("rearDist")) ultrasonicRearDistance = server.arg("rearDist").toInt();
  if (server.hasArg("lidarDist")) lidarDistanceThreshold = server.arg("lidarDist").toInt();

  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle form submission to set the wait mode timer.
*/
void handleWaitMode() {
  if (server.hasArg("wait")) waitTime = server.arg("wait").toInt();

  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle incoming client requests to the web server.
*/
void handleClientRequests() {
  server.handleClient();
}

#endif
