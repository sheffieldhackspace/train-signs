#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <BigClock.h>
#include <cppQueue.h>
#include <GFX_fonts/Font5x7Fixed.h>
#include <WiFi.h>

#include "Credentials.h"

typedef struct msgRec {
  String *message;
} Record;

cppQueue queue(sizeof(Record), 10, FIFO, true);
WiFiServer server(80);

GFXcanvas1 *canvas = NULL;
BigClock *display = NULL;

void setup() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  canvas = new GFXcanvas1(BIG_CLOCK_WIDTH, BIG_CLOCK_HEIGHT);
  canvas->setFont(&Font5x7Fixed);
  canvas->setTextSize(1);
  canvas->setTextColor(1);

  display = new BigClock(canvas->getBuffer());

  canvas->fillScreen(0);
  canvas->setCursor(0, 7);
  canvas->print("SSID: ");
  canvas->println(WIFI_SSID);
  canvas->println("Connecting...");
  display->output();

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }

  server.begin();

  Record record = { new String(WiFi.localIP().toString() + ":80") };
  queue.push(&record);
}

void loop() {
  String *message = new String();
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

      message->concat(c);
    }

    Record record = { message };
    queue.push(&record);
  }

  uint32_t time = millis() / 5000;
  int index = time % queue.getCount();

  Record record;
  queue.peekIdx(&record, index);
  message = record.message;

  canvas->fillScreen(0);
  canvas->setCursor(0, 7);
  canvas->println(*message);
  display->output();
  delay(50);
}
