/* checkerboard
Show a checkerboard pattern on the screen.
The corners are inverted
*/
#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <BigClock.h>

#ifdef __AVR__
#define BOARDSEL_PIN 2
#define LATCH_PIN 4
#define WOUT_PIN 6
#endif
#ifdef ESP8266
#define BOARDSEL_PIN 12 // D6
#define LATCH_PIN 13    // D7
#define WOUT_PIN 14     // D5
#endif

#define MAX_X 96
#define MAX_Y 26

GFXcanvas1 *canvas = NULL;
BigClock *bc = NULL;

void setup() {
    canvas = new GFXcanvas1(MAX_X, MAX_Y);
    bc = new BigClock();
    bc->init();

    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
            canvas->drawPixel(i, j, (i + j) % 2);
        }
    }

    uint8_t *buffer = canvas->getBuffer();

    bc->output(buffer);
}

void loop() {
}
