#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>  // For mDNS service to resolve hostname

// Wi-Fi credentials and settings
const char* ssid = "HoverKart";
const char* password = "kartpassword";
const char* hostname = "hoverkart";  // Hostname for the Wi-Fi AP

// Web server object on port 80
WebServer server(80);

// PID tuning parameters and following distance variables (these can be changed via the web interface)
double webKp = 2.5, webKi = 6.0, webKd = 1.2;
int followingDistance = 100;
int waitTime = 0;

// Function prototypes
void setupWiFi();
void handleRoot();
void handleUpdatePID();
void handleUpdateDistance();
void handleWaitMode();

/**
   Set up the Wi-Fi Access Point (AP) mode, assign the hostname, and start the web server.
*/
void setupWiFi() {
  WiFi.softAP(ssid, password);
  WiFi.softAPsetHostname(hostname);  // Set the hostname

  Serial.println("Wi-Fi started in AP mode.");
  Serial.println("Hostname: " + String(hostname));
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  // Start mDNS to resolve the hostname
  if (MDNS.begin(hostname)) {
    Serial.println("MDNS responder started");
    MDNS.addService("http", "tcp", 80);  // Add service to make it accessible via http
  } else {
    Serial.println("Error setting up MDNS responder.");
  }

  // Web server routes
  server.on("/", handleRoot);
  server.on("/updatePID", handleUpdatePID);
  server.on("/updateDistance", handleUpdateDistance);
  server.on("/waitMode", handleWaitMode);

  // Start the web server
  server.begin();
}

/**
   Root webpage that allows control of PID values and following distance.
*/
void handleRoot() {
  String page = "<html><head><title>Kart Control</title></head><body>";
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

  // Wait mode form
  page += "<form action='/waitMode' method='POST'>";
  page += "Wait Time (seconds): <input type='number' name='wait' value='" + String(waitTime) + "'><br>";
  page += "<input type='submit' value='Set Wait Mode'></form><br>";

  page += "</body></html>";

  // Send the page content
  server.send(200, "text/html", page);
}

/**
   Handle the form submission for updating PID values.
*/
void handleUpdatePID() {
  if (server.hasArg("kp")) webKp = server.arg("kp").toDouble();
  if (server.hasArg("ki")) webKi = server.arg("ki").toDouble();
  if (server.hasArg("kd")) webKd = server.arg("kd").toDouble();

  // Redirect back to the main page
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle the form submission for updating the following distance.
*/
void handleUpdateDistance() {
  if (server.hasArg("distance")) followingDistance = server.arg("distance").toInt();

  // Redirect back to the main page
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Handle the form submission for setting the wait mode timer.
*/
void handleWaitMode() {
  if (server.hasArg("wait")) waitTime = server.arg("wait").toInt();

  // Redirect back to the main page
  server.sendHeader("Location", "/");
  server.send(303);
}

/**
   Call this function in the main loop to handle HTTP requests.
*/
void handleClientRequests() {
  server.handleClient();
}

#endif
