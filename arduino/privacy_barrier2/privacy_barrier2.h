#ifndef PRIVACY_BARRIER_H
#define PRIVACY_BARRIER_H

#include <FastLED.h>
#include "ledBlock.h"

// determines if the setup is test or "real" mode
#define REAL

FASTLED_USING_NAMESPACE

int debugLevel = 0;

// Digital
#define LED_CONTROL_PIN        3
#define LED_PROGRAM_TOGGLE_PIN 2
#define HIGH_LIGHT_LEVEL_LED_PIN 4

#if defined(REAL)
#define NUM_LEDS           84
#else
#define NUM_LEDS          6
#endif

#define MAX_BRIGHTNESS     255
#define COLOR_ORDER        GRB
#define LED_TYPE           WS2812B

#if defined(REAL)
#define NUM_LED_BLOCKS  16
#else
#define NUM_LED_BLOCKS  3
#endif

ledBlock g_led_block[NUM_LED_BLOCKS];
bool g_active_led_block_list[NUM_LED_BLOCKS];

// Analog
const int g_BRIGHTNESS_CONTROL_POT_PIN = A0; 
const int g_COLOR_CONTROL_POT_PIN = A1;
const int g_LIGHT_SENSOR_INPUT_PIN = A2;

// pattern_id values - new patterns will get added in here
enum
{
  SIMPLE_TEST,
  TWO_COLOR_RANDOM_LED_BLOCKS1,
  TWO_COLOR_RANDOM_LED_BLOCKS2,
  AVALANCHE_COLOR_RANDOM_LED_BLOCKS,
  NUGGETS_COLOR_RANDOM_LED_BLOCKS,
  BRONCOS_COLOR_RANDOM_LED_BLOCKS,
  CUBS_COLOR_RANDOM_LED_BLOCKS,
  SAME_COLOR_RANDOM_LED_BLOCKS,
  DIFFERENT_COLOR_RANDOM_LED_BLOCKS,
  SINGLE_COLOR,
  TEMPORAL_RAINBOW,
  STATIC_WHITE,
  MAX_NUMBER_LED_PATTERNS
};

CRGB g_leds[NUM_LEDS];

int g_target_number_active_led_blocks;
float g_percentage_active_led_blocks = 0.7;
int g_just_inactive_led_block_list[NUM_LED_BLOCKS];
int g_num_inactive_led_blocks = 0;

// if above threshold, LEDs go off, below, they go on
// good value 10/10/2025

#if defined(REAL)
int g_lightSensorThreshold=30;
//int g_lightSensorThreshold=1000;
#else
int g_lightSensorThreshold=500;
#endif

int g_currentLightLevel;

int g_currentBrightnessLevel=0;
// need to adjust this 10/10/2025
int g_maxBrightnessLevel=1023;
float g_defaultBrightnessChangeRate = 1.0;
int g_maxTimeAtBrightnessLevelMS = 3000;
float g_brightnessChangeAmount = 0.005;


int g_currentColorLevel=0;
// the pot is very non-linear, at the end of the range so above this threshold, clamp the current level
int g_maxColorLevel=73;

int g_currentLEDProgramID=0;
bool g_lastLEDProgramButton = LOW;

int g_color_skip=20;
//CRGB *colorRange;
CRGB g_colorSample[73];
int g_total_number_of_colors;
CRGB g_no_color;
#endif
