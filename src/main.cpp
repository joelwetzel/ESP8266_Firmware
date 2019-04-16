#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SSD1306Wire.h"

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"

SSD1306Wire display(0x3c, 4, 5, GEOMETRY_128_32);

String configAccessPoint("ESP8266");

int i = 0;

class DisplayState
{
  public:
    String Line1;
    String Line2;
    String Line3;

    DisplayState()
    {
      Line1 = "Connecting...";
      Line2 = "";
      Line3 = "";
    }
};

DisplayState displayState;


void resetOledDisplay() {
  //Manual reset of the OLED display
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void renderToDisplay() {
  display.clear();

  display.drawString(0, 0, displayState.Line1);
  display.drawString(0, 11, displayState.Line2);
  display.drawString(0, 22, displayState.Line3);

  display.display();
}

void configModeCallback (WiFiManager *wifiManager)
{
  displayState.Line1 = "Connect to " + configAccessPoint + " to";
  displayState.Line2 =  "configure this device.";
  renderToDisplay();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  resetOledDisplay();

  renderToDisplay();

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(60);
  //wifiManager.resetSettings();
  wifiManager.autoConnect(configAccessPoint.c_str());

  if (WiFi.localIP().isSet())
  {
    displayState.Line1 = "SSID: " + WiFi.SSID();
    displayState.Line2 = "IP: " + WiFi.localIP().toString();
    renderToDisplay();
  }
  else
  {
    displayState.Line1 = "Rebooting...";
    displayState.Line2 = "";
    renderToDisplay();

    delay(2000);

    ESP.restart();
  }  
}



void loop()
{
  i++;

  String strI(i);
  displayState.Line3 = strI;

  renderToDisplay();

  delay(1000);
}