#include <DotWidget.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Fonts/Org_01.h>
#include <WiFi.h>

#include "Credentials.h"

constexpr const char *HTTP_SEPARATOR = "\r\n\r\n";
constexpr size_t MAX_PAYLOAD_SIZE = 4096;
constexpr int SERVER_PORT = 80;

constexpr const char *RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_400 = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
constexpr const char *RESPONSE_413 = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";

inline uint8_t getAlign(std::string_view text) {
  if (text == "left") return ALIGN_LEFT;
  if (text == "center") return ALIGN_CENTER;
  if (text == "right") return ALIGN_RIGHT;
  if (text == "top") return ALIGN_TOP;
  if (text == "middle") return ALIGN_MIDDLE;
  if (text == "bottom") return ALIGN_BOTTOM;

  return 0;
}

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

  widget.setAlign(ALIGN_CENTER | ALIGN_MIDDLE);
  widget.setInverted(true);
  widget.setText("SSID: " + WIFI_SSID + "\nConnecting...", &Org_01);
  widget.render();
  display.display();

  while (!WiFi.isConnected()) {
    if (WiFiClass::status() == WL_DISCONNECTED) {
      WiFi.reconnect();
    }

    delay(1000);
  }

  server.begin();

  widget.setAlign(ALIGN_CENTER | ALIGN_MIDDLE);
  widget.setInverted(false);
  widget.setText("SSID: " + WIFI_SSID + "\n" + WiFi.localIP().toString() + ":" + SERVER_PORT, &Org_01);
  widget.render();
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

      widget.setFlashing(document["flashing"] | false);
      widget.setInverted(document["inverted"] | false);
      widget.setSpeed(document["speed"] | 5);

      widget.setAlign(getAlign(document["horizontal_align"] | "left") | getAlign(document["vertical_align"] | "top"));
      widget.setImage(document["image"] | "", document["image_width"] | 0, document["image_height"] | 0);
      widget.setText(document["text"] | "", &Org_01, document["text_size"] | 1, document["text_wrap"] | true);

      client.print(RESPONSE_200);
      client.stop();
    }
  }

  widget.render();
  widget.advanceFrame();
  display.display();

  if (millis() - last >= 5000) {
    Serial.printf("Free: %dB, Min free: %dB\n", ESP.getFreeHeap(), ESP.getMinFreeHeap());
    last = millis();
  }
}
