/* microswitch
Check that a button is read by the Arduino
Internal LED state is set to button state
It should turn on when a button is pressed.
Button to be wired to GND and PIN 3
*/

#include <Arduino.h>

#define LED_PIN 13
#define SWITCH_PIN 3

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void loop()
{
    // read the microswitch
    int val = digitalRead(SWITCH_PIN);

    // write to LED
    digitalWrite(LED_PIN, val);
}
