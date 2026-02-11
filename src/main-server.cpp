#include <Adafruit_BigClock.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Widget.h>
#include <WiFi.h>

#include "Credentials.h"

#ifndef HTTP_SEPARATOR
#define HTTP_SEPARATOR "\r\n\r\n"
#endif

WiFiServer server(80);

Widget *widget = new Widget(
  new Adafruit_BigClock(
    new Adafruit_BigClockSPI(D2, D4, D6, D8),
    new Adafruit_BigClockSPI(D1, D3, D5, D7)
  )
);

void setup() {
  widget->begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  widget->setInvert(true);
  widget->setMessage(new String("SSID: " + WIFI_SSID + "\nConnecting..."));
  widget->display();

  while (!WiFi.isConnected()) {
    if (WiFiClass::status() == WL_DISCONNECTED) {
      WiFi.reconnect();
    }

    delay(1000);
  }

  server.begin();

  widget->setInvert(false);
  widget->setMessage(new String("SSID: " + WIFI_SSID + "\n" + WiFi.localIP().toString() + ":80"));
  widget->display();
}

void loop() {
  if (WiFiClient client = server.accept()) {
    while (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;
      deserializeJson(document, client);

      widget->setInvert(document["invert"] | false);
      widget->setMessage(new String(document["message"] | ""));
      widget->setSpeed(document["speed"] | 5);
      widget->setTextAlign(document["text_align"] | LEFT);
      widget->setVerticalAlign(document["vertical_align"] | TOP);
    }
  }

  widget->display();
  widget->waitForNextFrame();
}
