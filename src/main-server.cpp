#include <Adafruit_BigClock.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Widget.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "Credentials.h"

const char *HTTP_SEPARATOR = "\r\n\r\n";
const size_t MAX_PAYLOAD_SIZE = 4096;
const int SERVER_PORT = 80;

const char *RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
const char *RESPONSE_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
const char *RESPONSE_413 = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

WiFiServer server(SERVER_PORT);

Adafruit_BigClockSPI spi1(D1, D3, D5, D8);
Adafruit_BigClockSPI spi2(D0, D2, D4, D7);
Adafruit_BigClock big_clock(&spi1, &spi2);
Adafruit_Widget widget(&big_clock);

void setup() {
  big_clock.begin();
  widget.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  widget.setHorizontalAlign(CENTER);
  widget.setVerticalAlign(MIDDLE);
  widget.setInvert(true);
  widget.setText(String("SSID: " + WIFI_SSID + "\nConnecting..."));
  widget.print();
  big_clock.display();

  while (!WiFi.isConnected()) {
    if (WiFiClass::status() == WL_DISCONNECTED) {
      WiFi.reconnect();
    }

    delay(1000);
  }

  server.begin();

  widget.setInvert(false);
  widget.setText(String("SSID: " + WIFI_SSID + "\n" + WiFi.localIP().toString() + ":" + SERVER_PORT));
  widget.print();
  big_clock.display();
}

void loop() {
  if (WiFiClient client = server.accept()) {
    if (client.available() > MAX_PAYLOAD_SIZE) {
      client.print(RESPONSE_413);
      client.stop();
    } else if (client.available()) {
      client.find(HTTP_SEPARATOR);

      JsonDocument document;

      if (deserializeJson(document, client)) {
        client.print(RESPONSE_400);
        client.stop();
        return;
      }

      widget.setImage(String(document["image"] | ""), document["image_width"] | 0, document["image_height"] | 0);
      widget.setText(String(document["text"] | ""));

      widget.setFlash(document["flash"] | false);
      widget.setInvert(document["invert"] | false);
      widget.setSpeed(document["speed"] | 5);
      widget.setTextWrap(document["text_wrap"] | true);

      widget.setHorizontalAlign(document["horizontal_align"] | LEFT);
      widget.setVerticalAlign(document["vertical_align"] | TOP);

      client.print(RESPONSE_200);
      client.stop();
    }
  }

  widget.print();
  widget.advanceFrame();
  big_clock.display();
}
