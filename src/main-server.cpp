#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <BigClock.h>
#include <GFX_fonts/Font5x7Fixed.h>
#include <WiFi.h>

#include "Credentials.h"

WiFiServer server(80);

GFXcanvas1 *canvas = NULL;
uint8_t *buffer = NULL;
BigClock *display = NULL;

String message;

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas = new GFXcanvas1(96, 26);
  canvas->setFont(&Font5x7Fixed);
  canvas->setTextSize(1);
  canvas->setTextColor(1);
  buffer = canvas->getBuffer();

  display = new BigClock();
  display->init();

  while (WiFi.status() != WL_CONNECTED) {
    canvas->fillScreen(0);
    canvas->setCursor(0, 7);
    canvas->print("SSID: ");
    canvas->println(WIFI_SSID);
    canvas->println("Connecting...");
    display->output(buffer);

    delay(50);
  }

  server.begin();

  message = WiFi.localIP().toString();
  message.concat(":80");
}

void loop() {
  if (WiFiClient client = server.accept()) {
    int b = 0;
    message = String();

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

      message.concat(c);
    }
  }

  canvas->fillScreen(0);
  canvas->setCursor(0, 7);
  canvas->println(message);
  display->output(buffer);
  delay(50);
}
