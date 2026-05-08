#include <Arduino.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 21
#define DATA_PIN_0 9
#define DATA_PIN_1 10

CRGB leds[2][NUM_LEDS];


void setup() 
{
  Serial.begin(9600);
  delay(1000);
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE,DATA_PIN_0,COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN_1,COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[0][i].red = 255;
    leds[0][i].green=0;
    leds[0][i].blue=0;
  }
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[1][i].red = 0;
    leds[1][i].green=255;
    leds[1][i].blue=0;
  }

  FastLED.show();
  FastLED.delay(10); 


}
