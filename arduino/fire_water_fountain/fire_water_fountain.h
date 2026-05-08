#ifndef FIRE_WATER_FOUNTAIN_H
#define FIRE_WATER_FOUNTAIN_H

#include <FastLED.h>

int debugLevel = 1;

#define LED_PROGRAM_TOGGLE A0
#define PUMP_PROGRAM_TOGGLE A1


// order of pumps goes from left to right so pump 0 is leftmost
#define NUMBER_OF_PUMPS 3
#define NUMBER_OF_LED_LIGHTS 3
#define LEFT 0
#define CENTER 1
#define RIGHT 2

// pump io
// leftmost
#define LEFT_PUMP_INLO_PIN 4
#define LEFT_PUMP_INHI_PIN 2
#define LEFT_PUMP_EN_PIN 3

// center
#define CENTER_PUMP_INLO_PIN 8
#define CENTER_PUMP_INHI_PIN 7
#define CENTER_PUMP_EN_PIN 5

// right
#define RIGHT_PUMP_INLO_PIN 13
#define RIGHT_PUMP_INHI_PIN 12
#define RIGHT_PUMP_EN_PIN 6


// led_io
#define LEFT_LED_CONTROL_PIN 9
#define CENTER_LED_CONTROL_PIN 10
#define RIGHT_LED_CONTROL_PIN 11
#define NUMBER_LEDS_PER_LIGHT 21


// pump programs
enum
{
  PUMPS_CHANGING_ANGLE,
  MAX_RAW_HEIGHTS,
  MAX_HEIGHTS,
  PUMPS_RANDOM_SYNCHED_UP_DOWN,
  PUMPS_RANDOM,
  ALTERNATING_HEIGHTS,
  MAX_PUMP_PROGRAMS
};

// led programs
enum
{
  LED_TEMPORAL_SPECTRUM,
  LED_RANDOM,
  LED_RANDOM_SYNCED,
  AVS_COLORS,
  NUGGETS_COLORS,
  CUBS_COLORS,
  BRONCOS_COLORS,
  MAX_LED_PROGRAMS
};

#endif