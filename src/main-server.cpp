#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <BigClock.h>
#include <WiFi.h>

#include "Credentials.h"

WiFiServer server(80);

GFXcanvas1 *canvas = NULL;
uint8_t *buffer = NULL;
BigClock *display = NULL;

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas = new GFXcanvas1(96, 26);
  canvas->setTextSize(0);
  canvas->setTextColor(1);
  buffer = canvas->getBuffer();

  display = new BigClock();
  display->init();

  while (WiFi.status() != WL_CONNECTED) {
    canvas->fillScreen(0);
    canvas->setCursor(0, 0);
    canvas->println("WiFi...");
    display->output(buffer);

    delay(100);
  }

  server.begin();
}

void loop() {
  canvas->fillScreen(0);
  canvas->setCursor(0, 0);
  canvas->print(WiFi.localIP());
  canvas->println(":80");

  if (WiFiClient client = server.accept()) {
    int b = 0;

    while (client.available()) {
      char c = client.read();

      if (b < 4) {
        if (c == 10 || c == 13) {
          b++;
        } else {
          b = 0;
        }

        continue;
      }

      canvas->print(c);
    }
  }

  display->output(buffer);
  delay(1000);
}
