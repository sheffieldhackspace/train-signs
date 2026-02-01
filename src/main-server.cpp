#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <BigClock.h>
#include <Org01Condensed.h>
#include <WiFi.h>

#include "Credentials.h"

#ifndef HTTP_SEPARATOR
#define HTTP_SEPARATOR "\r\n\r\n"
#endif

WiFiServer server(80);

int8_t alignv = -1;
bool background = 0;
String *message = NULL;
uint8_t speed = 5;

GFXcanvas1 *canvas = NULL;
BigClock *display = NULL;

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas = new GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT);
  canvas->setFont(&Org01Condensed);
  canvas->setTextSize(1);

  display = new BigClock(canvas->getBuffer());

  canvas->fillScreen(1);
  canvas->setTextColor(0);
  canvas->setCursor(0, 4);
  canvas->print("SSID: ");
  canvas->println(WIFI_SSID);
  canvas->println("Connecting...");
  display->output();

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

      alignv = document["alignv"] | -1;
      background = document["background"] | 0;
      message = new String(document["text"]);
      speed = document["speed"] | 1;
    }
  }

  int16_t x, y;
  uint16_t w, h, offset = 0, frame = 0;

  canvas->getTextBounds(*message, 0, 0, &x, &y, &w, &h);

  int16_t start = 4;
  int16_t stop;
  int16_t frames = 40 + stop;

  if (h > BIG_CLOCK_HEIGHT) {
    stop = 1 + h - BIG_CLOCK_HEIGHT;
    frame = (millis() / (1000 / speed)) % frames;

    if (frame < 20) {
      offset = 0;
    } else if (frame < 20 + stop) {
      offset = frame - 20;
    } else {
      offset = stop;
    }
  } else {
    stop = BIG_CLOCK_HEIGHT - h;
    switch (alignv) {
      case -1:
        offset = 0;
        break;
      case 0:
        offset = -(stop / 2);
        break;
      case 1:
        offset = -(stop);
        break;
    }
  }

  canvas->setTextColor(!background);
  canvas->fillScreen(background);
  canvas->setCursor(0, start - offset);
  canvas->println(*message);
  display->output();
  delay(1000 / speed);
}
