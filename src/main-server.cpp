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

enum VERTICAL_ALIGNMENT {
  TOP = -1,
  CENTER = 0,
  BOTTOM = 1,
};

VERTICAL_ALIGNMENT alignv = TOP;
bool background = 0;

Widget *widget = new Widget(
  new Adafruit_BigClock(
    new Adafruit_BigClockSPI(D2, D4, D6, D8),
    new Adafruit_BigClockSPI(D1, D3, D5, D7)
  )
);

void setup() {
  widget->begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  widget->setMessage(new String("SSID: " + WIFI_SSID + "\nConnecting..."));
  widget->display();

  while (!WiFi.isConnected()) {
    delay(50);
  }

  server.begin();

  widget->setMessage(new String("SSID: " + WIFI_SSID + "\n" + WiFi.localIP().toString() + ":80"));
  widget->display();
}

void loop() {
  if (WiFiClient client = server.accept()) {
    while (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;
      deserializeJson(document, client);

      alignv = document["alignv"] | TOP;
      background = document["background"] | 0;
      widget->setMessage(new String(document["text"]));
      widget->setSpeed(document["speed"] | 5);
    }
  }

  // if (h > BC_HEIGHT) {
  //   int16_t stop = h - BC_HEIGHT;
  //   int16_t frames = 40 + stop;
  //   frame = frame >= frames ? 0 : frame + 1;
  //
  //   if (frame < 20) {
  //     offset = 0;
  //   } else if (frame - 20 < stop) {
  //     offset = -(frame - 20);
  //   } else {
  //     offset = -stop;
  //   }
  // } else {
  //   switch (alignv) {
  //     case TOP:
  //       offset = 0;
  //       break;
  //     case CENTER:
  //       offset = (BC_HEIGHT - h) / 2;
  //       break;
  //     case BOTTOM:
  //       offset = BC_HEIGHT - h;
  //       break;
  //   }
  // }
  //
  // canvas->setCursor(0, start + offset);
  widget->display();
  widget->waitForNextFrame();
}
