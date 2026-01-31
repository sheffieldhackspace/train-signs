#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <BigClock.h>
#include <MessageQueue.h>
#include <Org01Condensed.h>
#include <WiFi.h>

#include "Credentials.h"

MessageQueue *queue = new MessageQueue();
WiFiServer server(80);

GFXcanvas1 *canvas = NULL;
BigClock *display = NULL;

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas = new GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT);
  canvas->setFont(&Org01Condensed);
  canvas->setTextSize(1);
  canvas->setTextColor(1);

  display = new BigClock(canvas->getBuffer());

  canvas->fillScreen(0);
  canvas->setCursor(0, 5);
  canvas->print("SSID: ");
  canvas->println(WIFI_SSID);
  canvas->println("Connecting...");
  display->output();

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  server.begin();

  String *message = new String();
  message->concat("SSID: ");
  message->concat(WIFI_SSID);
  message->concat("\n");
  message->concat(WiFi.localIP().toString());
  message->concat(":80");

  queue->push(new Record(message));
}

void loop() {
  if (WiFiClient client = server.accept()) {
    String *message = new String();
    int b = 0;
    char c;

    while (client.available()) {
      c = client.read();

      if (b < 4) {
        if (c == 10 || c == 13) {
          b++;
        } else {
          b = 0;
        }

        continue;
      }

      message->concat(c);
    }

    queue->push(new Record(message));
  }

  canvas->fillScreen(0);
  canvas->setCursor(0, 5);
  canvas->println(*queue->getCurrent()->message);
  display->output();
  delay(50);
}
