#include <DotWidget.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "Credentials.h"

constexpr const char *HTTP_SEPARATOR = "\r\n\r\n";
constexpr size_t MAX_PAYLOAD_SIZE = 4096;
constexpr int SERVER_PORT = 80;

constexpr const char *RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_413 = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

unsigned long last = 0;
WiFiServer server(SERVER_PORT);

#ifdef USE_OLED
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#else
#include <AEGMIS_GV60.h>
AEGMIS_GV60_SPI spi1(D1, D3, D5, D8);
AEGMIS_GV60_SPI spi2(D0, D2, D4, D7);
AEGMIS_GV60 display(&spi1, &spi2);
#endif

DotWidget widget(&display);

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

#ifdef USE_OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else
  display.begin();
#endif

  widget.begin();
  widget.setHorizontalAlign(CENTER);
  widget.setVerticalAlign(MIDDLE);
  widget.setInvert(true);
  widget.setText(String("SSID: " + WIFI_SSID + "\nConnecting..."));
  widget.print();
  display.display();

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
  display.display();
}

void loop() {
  if (auto client = server.accept()) {
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

      widget.setImage(document["image"] | "", document["image_width"] | 0, document["image_height"] | 0);
      widget.setText(document["text"] | "");

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
  display.display();

  if (millis() - last >= 5000) {
    Serial.printf("Free: %dB, Min free: %dB\n", ESP.getFreeHeap(), ESP.getMinFreeHeap());
    last = millis();
  }
}
