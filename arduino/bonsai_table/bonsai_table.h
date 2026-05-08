#ifndef BONSAI_TABLE_H
#define BONSAI_TABLE_H

#include <FastLED.h>

FASTLED_USING_NAMESPACE

int debugLevel = 1;

// Digital
#define LED_CONTROL_PIN        4
#define LED_PROGRAM_TOGGLE_PIN 2

//#define NUM_LEDS           20
#define NUM_LEDS          8
#define MAX_BRIGHTNESS     255
#define COLOR_ORDER        GRB
#define LED_TYPE           WS2812B

#define NUM_LED_BLOCKS  4


// Analog
const int BRIGHTNESS_CONTROL_POT_PIN = A0; 
const int COLOR_CONTROL_POT_PIN = A2;
const int LIGHT_SENSOR_INPUT_PIN = A4;

// pattern_id values - new patterns will get added in here
enum
{
  SINGLE_COLOR,
  //ROTATING_COLOR,
  TEMPORAL_RAINBOW,
  MAX_NUMBER_LED_PATTERNS
};

CRGB leds[NUM_LEDS];

int number_leds_per_block = 2;

// if above threshold, LEDs go off, below, they go on
int lightSensorThreshold=30;
int currentLightLevel;

int currentBrightnessLevel=0;
int maxBrightnessLevel=1017;

int currentColorLevel=0;
int maxColorLevel=1017;

int currentLEDProgramID=0;
bool lastLEDProgramButton = LOW;

int color_skip=5;
//CRGB *colorRange;
CRGB *colorSample;
int total_number_of_colors;
CRGB no_color;
#endif
