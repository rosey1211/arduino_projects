#include <Arduino.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define COLOR_ORDER GRB
#define LED_TYPE    WS2812B

#define DATA_PIN     3  // Replace with your actual data pin
//#define NUM_LEDS    79  // Replace with your number of LEDs
#define NUM_LEDS    6  // Replace with your number of LEDs

CRGB leds[NUM_LEDS];

#define MY_RED CRGB::Green
#define MY_GREEN CRGB::Red
#define MY_BLUE CRGB::Blue

void setup() {


  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS); // Or use NEOPIXEL instead of WS2812B
  // GRB is the color order for WS2815. If your LEDs are a different order, you may need to adjust this.

  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i].red=0;
    leds[i].green=0;
    leds[i].blue=0;
  }

  FastLED.show();
}

void loop() {
  // Example: Cycle through colors
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = MY_RED;
  }
  FastLED.show();
  delay(1000);
#if 1
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = MY_GREEN;
  }
  FastLED.show();
  delay(1000);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = MY_BLUE;
  }
  FastLED.show();
  delay(1000);
#endif  
}

