#include <Arduino.h>
#include <FastLED.h>
#include "privacy_barrier2.h"



void setup()
{
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_PROGRAM_TOGGLE_PIN, INPUT);
  pinMode(HIGH_LIGHT_LEVEL_LED_PIN, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_CONTROL_PIN, COLOR_ORDER>(g_leds, NUM_LEDS); 
  // GRB is the color order for WS2815. If your LEDs are a different order, you may need to adjust this.

  // set master brightness control
  FastLED.setBrightness(MAX_BRIGHTNESS);
  
  // set all LEDs to off at start
  for(int i=0;i<NUM_LEDS;i++)
  {
    g_leds[i].red=0;
    g_leds[i].green=0;
    g_leds[i].blue=0;
  }

  FastLED.show();

  // set one above threshold so LEDs are initially off
  g_currentLightLevel = g_lightSensorThreshold+1;

  setupColorRange();

  setupLEDBlocks();
}


void loop()
{
  // if in simple test mode, then just turn all LEDs to green - this helps prove led connectivity 
  if(g_currentLEDProgramID==SIMPLE_TEST)
  {
    for(int i=0;i<NUM_LEDS;i++)
    {
      g_leds[i].red=255;
      g_leds[i].green=0;
      g_leds[i].blue=0;
    }
    FastLED.show();
    delay(100);
  }  
  


 // determine if the LED program toggle was triggered
  if(readLEDProgramButton() && g_currentLEDProgramID!=SIMPLE_TEST)
  {
    for(int i=0;i<NUM_LEDS;i++)
    {
      g_leds[i] = g_no_color;
    }

    FastLED.show();
    delay(500);
  }



  // this runs independent of the light level so do before light level logic
  if(g_currentLEDProgramID==SEQUENTIAL_BLOCK_TEST)
  {
    //Serial.print("target active blocks = ");
    //Serial.print((int)g_target_number_active_led_blocks);
    //Serial.print("\n");
    for(int b=0;b<NUM_LED_BLOCKS;b++)
    {
      Serial.print("led block: ");
      Serial.print(b);
      Serial.print("\n\n");
      for(int i=0;i<NUM_LEDS;i++)
      {
        g_leds[i].red=0;
        g_leds[i].green=255;
        g_leds[i].blue=0;
      }
  
  #if 1
      unsigned int start_led=g_led_block[b].m_startLED;
      unsigned int end_led=g_led_block[b].m_endLED;
      //Serial.print((int)start_led);
      //Serial.print((int)end_led);
      //Serial.print("\n");
      for(int l=(int)start_led;l<=(int)end_led;l++)
      {
        g_leds[l].red=255;
        g_leds[l].green=0;
        g_leds[l].blue=0;
      }
  #endif
  
      FastLED.show();
      delay(5000);
    }
    //Serial.print("\n\n\n");
    return;
  }


  // read the light sensor
  g_currentLightLevel = analogRead(g_LIGHT_SENSOR_INPUT_PIN);
  if(debugLevel>2)
  {
    Serial.print("light value = ");
    Serial.print(g_currentLightLevel);
    Serial.print("\n");
  }
 if(g_currentLightLevel>g_lightSensorThreshold && g_currentLEDProgramID!=SIMPLE_TEST)
  {
    for(int i=0;i<NUM_LEDS;i++)
    {
      g_leds[i] = g_no_color;
    }

    digitalWrite(HIGH_LIGHT_LEVEL_LED_PIN, HIGH);   
    FastLED.show();
    delay(100);
    return;
  }
  
  digitalWrite(HIGH_LIGHT_LEVEL_LED_PIN, LOW);   

  // read the brightness pot
  g_currentBrightnessLevel = analogRead(g_BRIGHTNESS_CONTROL_POT_PIN);  
  if(g_currentBrightnessLevel>g_maxBrightnessLevel)
  {
    // clipping color level to deal with pot non-linearities
    g_currentBrightnessLevel=g_maxBrightnessLevel;
  }

  // invert brightness since the pot seems flipped
  g_currentBrightnessLevel = g_maxBrightnessLevel-g_currentBrightnessLevel;

  if(debugLevel>0)
  {
    Serial.print("brighness value = ");
    Serial.print(g_currentBrightnessLevel);
    Serial.print("\n");
  }
  // read the color pot
  // read the brightness pot
  g_currentColorLevel = analogRead(g_COLOR_CONTROL_POT_PIN);  
  if(g_currentColorLevel>g_maxColorLevel)
  {
    // clipping color level to deal with pot non-linearities
    g_currentColorLevel=g_maxColorLevel;
  }
  if(debugLevel>2)
  {
    Serial.print("color value = ");
    Serial.print(g_currentColorLevel);
    Serial.print("\n");
  }

 
  // update color value
  runLEDProgram();
  //delay(100);
}
