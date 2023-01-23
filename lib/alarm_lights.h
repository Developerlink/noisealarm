#include <M5Stack.h>
#include <Adafruit_NeoPixel.h>

#ifndef ALARM_LIGHTS
#define ALARM_LIGHTS

#define M5STACK_FIRE_NEO_NUM_LEDS 10
#define M5STACK_FIRE_NEO_DATA_PIN 15

Adafruit_NeoPixel LED = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

void showAlarm() {
    LED.setPixelColor(0, LED.Color(255, 0, 0));
    LED.setPixelColor(1, LED.Color(255, 0, 0));
    LED.setPixelColor(2, LED.Color(255, 0, 0));
    LED.setPixelColor(3, LED.Color(255, 0, 0));
    LED.setPixelColor(4, LED.Color(255, 0, 0));
    LED.show();  
}

void turnOffAlarm() {
    LED.setPixelColor(0, LED.Color(0, 0, 0));
    LED.setPixelColor(1, LED.Color(0, 0, 0));
    LED.setPixelColor(2, LED.Color(0, 0, 0));
    LED.setPixelColor(3, LED.Color(0, 0, 0));
    LED.setPixelColor(4, LED.Color(0, 0, 0));
    LED.show();  
}

#endif