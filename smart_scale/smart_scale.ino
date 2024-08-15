#include <Arduino.h>
#include "HX711.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "soc/rtc.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 17;
const int LOADCELL_SCK_PIN = 16;

HX711 scale;

//WIFI connectivity
const char* ssid = "";
const char* password = "";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  //Connecting to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status()!=WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi... :)");
  Serial.println(WiFi.localIP());

// Initialise scale
  scaleSetup();
// Serve weight readings
  serveContent();

}
  

void loop() {
  
}