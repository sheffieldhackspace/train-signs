/**
 * ethernet sign
 *
 * listen to telnet and send text to sign
 * send things with e.g.,
 *   echo -e 'I am on a serial port!\0' | netcat 192.168.0.31 80
 *   printf 'AI IS FOR LOSERS\nTHINK FOR YOURSELF\0' | netcat 192.168.0.31 80
 *
 * currently, wiznet ethernet fails if an spi transaction is made to the sign
 *  don't know why or how to fix it :[
 */

// Arduino
#include <Arduino.h>

// Ethernet
#define W5500_ETHERNET_SHIELD
#include <Ethernet.h>
#include <SPI.h>
byte mac[] = {0x60, 0xF2, 0xEC, 0x18, 0x05, 0xFE};
EthernetServer server(80);
String message;
uint32_t messageStartTime = 0;

// Bus sign
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <BigClock.h>
#define MAX_X_DEVIATION 36
#define MAX_Y_DEVIATION 9
GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;
uint8_t *buffer;

// functions
String IPtoString(IPAddress &ip);
void beforeSign();
void afterSign();

void setup() {
  // can't use INBUILT LED as it's pin 13, which is used by SPI for Ethernet
  // pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, HIGH);

  // set up serial
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  Serial.println("starting...!");

  // set up sign
  afterSign();
  beforeSign();
  canvas = new GFXcanvas1(96, 26);
  bc = new BigClock();
  bc->init();
  canvas->fillScreen(0);
  canvas->setCursor(1, 1);
  canvas->println("await ethernet");
  canvas->println("check connection");
  canvas->println("reset if fail");
  buffer = canvas->getBuffer();
  bc->output(buffer);
  delay(500);
  bc->output(buffer);
  afterSign();
  delay(1000);

  // set up ethernet
  // disable SD SPI
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.println(F("Starting ethernet"));
  while (!Ethernet.begin(mac)) {
    Serial.print(F("."));
    delay(200);
  }
  Serial.print(F("Connected to… "));
  Serial.print(Ethernet.localIP());
  Serial.print(F(" on "));
  Serial.println(Ethernet.gatewayIP());

  // start listening for clients
  Serial.println(F("starting server"));
  server.begin();
  delay(1000);
  Serial.println(F("server started"));

  // get IP address as char*
  IPAddress ipAddress = Ethernet.localIP();
  String ipAddress_str =
      (String(ipAddress[0]) + '.' + String(ipAddress[1]) + '.' +
       String(ipAddress[2]) + '.' + String(ipAddress[3]));
  char ipAddress_arr[ipAddress_str.length() + 1];
  ipAddress_str.toCharArray(ipAddress_arr, ipAddress_str.length() + 1);

  // show IP on sign
  beforeSign();
  canvas->fillScreen(0);
  canvas->setCursor(1, 1);
  canvas->println("await telnet to");
  canvas->println(ipAddress_arr);
  canvas->println("see help for how");
  buffer = canvas->getBuffer();
  bc->output(buffer);
  afterSign();
  delay(1000);
}

void loop() {
  // take input raw from telnet
  EthernetClient client = server.available();
  if (client) {
    Serial.println("---Got a client---");
    messageStartTime = millis();

    // compute each character (byte) sent to self
    int totalChars = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        totalChars++;
        // end of message, quit
        if (c == '\0') {
          break;
        }
        // ignore really long message, in case spammers ;]
        if (totalChars > 64) {
          break;
        }
        message.concat(c);
      }

      // timeout
      if (millis() - messageStartTime > 1000) {
        break;
      }
    }

    beforeSign();
    // debug: print message
    Serial.println("got message:");
    Serial.println(message);

    // feedback messages to client
    client.println("---got write request for lines---");
    client.println(message);
    client.println("---no further content---");

    // show message on sign
    char message_arr[message.length() + 1];
    message.toCharArray(message_arr, message.length() + 1);
    canvas->fillScreen(0);
    canvas->setCursor(1, 1);
    canvas->print(message_arr);
    buffer = canvas->getBuffer();
    bc->output(buffer);
    afterSign();
    delay(1000);

    // reset and disconnect client
    message = "";
    Serial.println("---finished, disconnecting client---");
    delay(1);
    client.stop();
  }
}

void afterSign() {
  // reset SPI for Wiznet to use
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void beforeSign() {
  // stop SPI transaction which Ethernet is using
  SPI.endTransaction();
}

String IPtoString(IPAddress &ip) { // IP v4 only
  String ips;
  ips.reserve(16);
  ips = ip[0];
  ips += '.';
  ips += ip[1];
  ips += '.';
  ips += ip[2];
  ips += '.';
  ips += ip[3];
  return ips;
}
