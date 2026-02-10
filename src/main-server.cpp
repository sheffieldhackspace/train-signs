#include <Adafruit_BigClock.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Org01Condensed.h>
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
String *message = NULL;
uint8_t speed = 5;
uint16_t frame = 0;

Adafruit_BigClock *canvas = new Adafruit_BigClock(
  new BigBoard(BOARD_TOP, D5, D3, D1),
  new BigBoard(BOARD_BOTTOM, D6, D4, D2),
  D7,
  D8
);

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas->setFont(&Org01Condensed);
  canvas->setTextSize(1);

  canvas->fillScreen(1);
  canvas->setTextColor(0);
  canvas->setCursor(0, 4);
  canvas->print("SSID: ");
  canvas->println(WIFI_SSID);
  canvas->println("Connecting...");
  canvas->display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  server.begin();

  message = new String();
  message->concat("SSID: ");
  message->concat(WIFI_SSID);
  message->concat("\n");
  message->concat(WiFi.localIP().toString());
  message->concat(":80");
}

void loop() {
  if (WiFiClient client = server.accept()) {
    while (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;
      deserializeJson(document, client);

      alignv = document["alignv"] | TOP;
      background = document["background"] | 0;
      message = new String(document["text"]);
      speed = document["speed"] | 1;
      frame = 0;
    }
  }

  int16_t x, y;
  uint16_t w, h;

  canvas->getTextBounds(*message, 0, 0, &x, &y, &w, &h);

  int16_t start = -y, offset = 0;

  if (h > BIG_CLOCK_HEIGHT) {
    int16_t stop = h - BIG_CLOCK_HEIGHT;
    int16_t frames = 40 + stop;
    frame = frame >= frames ? 0 : frame + 1;

    if (frame < 20) {
      offset = 0;
    } else if (frame - 20 < stop) {
      offset = -(frame - 20);
    } else {
      offset = -stop;
    }
  } else {
    switch (alignv) {
      case TOP:
        offset = 0;
        break;
      case CENTER:
        offset = (BIG_CLOCK_HEIGHT - h) / 2;
        break;
      case BOTTOM:
        offset = BIG_CLOCK_HEIGHT - h;
        break;
    }
  }

  canvas->setTextColor(!background);
  canvas->fillScreen(background);
  canvas->setCursor(0, start + offset);
  canvas->println(*message);
  canvas->display();

  delay(1000 / speed);
}
