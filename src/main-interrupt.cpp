/**
 * Test the ESP8266TimerInterrupt library for ESP
 * should be compiled with ESP
 */

#include <Arduino.h>
#include <ESP8266TimerInterrupt.h>

#ifndef ESP8266
#error "for ESP8266 only"
#endif

// #define LED 21
#define LED D4

ESP8266TimerInterrupt ITimer;

#define TIMER_INTERVAL_MS 1000

void IRAM_ATTR Timer0_ISR()
{

    digitalWrite(LED, !digitalRead(LED));
}
void setup()
{
    Serial.begin(115200);
    Serial.println("starting!");

    pinMode(LED, OUTPUT);

    // Interval in microsecs
    if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, Timer0_ISR))
    {
        // lastMillis = millis();
        Serial.print(F("Starting  ITimer OK, millis() = "));
        // Serial.println(lastMillis);
    }
    else
    {
        Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));
    }
}
void loop()
{
    // Do Nothing!
}