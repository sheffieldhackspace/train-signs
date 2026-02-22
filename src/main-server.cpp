#include <Adafruit_BigClock.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Widget.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "Credentials.h"

const char *HTTP_SEPARATOR = "\r\n\r\n";
const char *RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

WiFiServer server(80);

Adafruit_BigClock *big_clock = new Adafruit_BigClock(
  new Adafruit_BigClockSPI(D1, D3, D5, D8),
  new Adafruit_BigClockSPI(D0, D2, D4, D7)
);

Adafruit_Widget *widget = new Adafruit_Widget(big_clock);

void setup() {
  big_clock->begin();
  widget->begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  widget->setHorizontalAlign(CENTER);
  widget->setVerticalAlign(MIDDLE);
  widget->setInvert(true);
  widget->setText(new String("SSID: " + WIFI_SSID + "\nConnecting..."));
  widget->print();
  big_clock->display();

  while (!WiFi.isConnected()) {
    if (WiFiClass::status() == WL_DISCONNECTED) {
      WiFi.reconnect();
    }

    delay(1000);
  }

  server.begin();

  widget->setInvert(false);
  widget->setText(new String("SSID: " + WIFI_SSID + "\n" + WiFi.localIP().toString() + ":80"));
  widget->print();
  big_clock->display();
}

void loop() {
  if (WiFiClient client = server.accept()) {
    while (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;
      deserializeJson(document, client);

      widget->setImage(new String(document["image"] | ""), document["image_width"] | 0, document["image_height"] | 0);
      widget->setText(new String(document["text"] | ""));

      widget->setFlash(document["flash"] | false);
      widget->setInvert(document["invert"] | false);
      widget->setSpeed(document["speed"] | 5);
      widget->setTextWrap(document["text_wrap"] | true);

      widget->setHorizontalAlign(document["horizontal_align"] | LEFT);
      widget->setVerticalAlign(document["vertical_align"] | TOP);

      client.print(RESPONSE_200);
      client.stop();
    }
  }

  widget->print();
  widget->advanceFrame();
  big_clock->display();
}
