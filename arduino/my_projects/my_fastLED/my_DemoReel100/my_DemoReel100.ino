/// @file    DemoReel100.ino
/// @brief   FastLED "100 lines of code" demo reel, showing off some effects
/// @example DemoReel100.ino

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define NUM_LED_STRIPS 2
#define UPPER_DATA_PIN    5
#define LOWER_DATA_PIN    4
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define NUM_LEDS_UPPER    40
CRGB leds_upper[NUM_LEDS_UPPER];

#define NUM_LEDS_LOWER    40
CRGB leds_lower[NUM_LEDS_LOWER];

#define MAX_BRIGHTNESS     100
#define FRAMES_PER_SECOND  120

// for potentiometer
const int BRIGHTNESS_POT=0; // pot on analog pin 0
const int TIME_PERIOD_POT=1; // pot on analog pin 1
int brightness_pot_val = 0;
int time_period_pot_val = 0;

double max_pot_value = 1023.0;
double min_pot_value = 0.0;

#define CHECK_POTS_INTERVAL 100

bool debug = false;
bool superDebug = false;

bool use_upper_leds = true;
bool use_lower_leds = false;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  if(use_upper_leds)
  {
    FastLED.addLeds<LED_TYPE,UPPER_DATA_PIN,COLOR_ORDER>(leds_upper, NUM_LEDS_UPPER).setCorrection(TypicalLEDStrip);
  }
  if(use_lower_leds)
  {
    FastLED.addLeds<LED_TYPE,LOWER_DATA_PIN,COLOR_ORDER>(leds_lower, NUM_LEDS_LOWER).setCorrection(TypicalLEDStrip);
  }
  // set master brightness control
  FastLED.setBrightness(MAX_BRIGHTNESS);

  Serial.begin(9600);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
SimplePatternList gPatterns = { temporal_rainbow };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


int max_time_period_ms=30*1000/FRAMES_PER_SECOND;
int min_time_period_ms=1000/FRAMES_PER_SECOND;
int time_delay = min_time_period_ms;

int last_loop_time_ms = millis();
int read_pot_interval_ms = 2000; // every 2 seconds

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //'FastLED.delay(1000/FRAMES_PER_SECOND); 
  FastLED.delay(time_delay); 
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically

  int dynamic_pots_interval = max_time_period_ms + (min_time_period_ms - max_time_period_ms)*((double)time_period_pot_val/max_pot_value);
  
  if(debug)
  {
    char line[1000];
    sprintf(line,"dynamic_pots_interval = %d : time_delay = %d",dynamic_pots_interval,time_delay);
    Serial.println(line);
  }

  // do this every 2 seconds no matter what  
  int current_loop_time_ms = millis();
  int time_diff_ms = current_loop_time_ms - last_loop_time_ms;
  if(time_diff_ms >= read_pot_interval_ms)
  {
    last_loop_time_ms = current_loop_time_ms;
    brightness_pot_val = analogRead(BRIGHTNESS_POT);
    time_period_pot_val = analogRead(TIME_PERIOD_POT);


    // compute a brightness value based on the potentiomter reading
    int new_brightness;
    double brightness_ratio = (double)brightness_pot_val/(double)max_pot_value;

    time_delay = min_time_period_ms + (max_time_period_ms - min_time_period_ms)*((double)time_period_pot_val/(double)max_pot_value);
    new_brightness = MAX_BRIGHTNESS*brightness_ratio;

    if(superDebug)
      {
        Serial.write("brightness pot_val = ");
        Serial.println(brightness_pot_val);
        Serial.write("brightness_ratio = ");
        Serial.println(brightness_ratio);

        Serial.write("time_period pot_val = ");
        Serial.println(time_period_pot_val);
        Serial.write("time_delay = ");
        Serial.println(time_delay);
        Serial.write("current_brightness = ");
        Serial.println(new_brightness);  
      } 
    
    if(debug)
    {
      char line2[100];
      sprintf(line2,"new_brightness = %d : time_delay = %d", new_brightness, time_delay);
      Serial.println(line2);
    }

    FastLED.setBrightness(new_brightness);
    delay(100);
  }

 }

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
  
CRGB temporal_color(255,0,0);

void temporal_rainbow()
{
  fill_solid( leds_upper, NUM_LEDS_UPPER,temporal_color);
  if(temporal_color.red==255 && temporal_color.green<255 && temporal_color.blue==0)
  {
    // moving towards yellow from red
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards yellow from red");
      Serial.println(line);
    } 

    temporal_color.green++;
  }
  else if(temporal_color.green==255 && temporal_color.red>0 && temporal_color.blue==0)
  {
    // moving towards pure green from yellow
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards pure green from yellow");    
      Serial.println(line);
    }

    temporal_color.red--;
  }
  else if(temporal_color.green==255 && temporal_color.red==0 && temporal_color.blue<255)
  {
    //moving towards turquoise from green
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards turquoise from green");
      Serial.println(line);
    }

    temporal_color.blue++;
  }
  else if(temporal_color.blue==255 && temporal_color.green>0 && temporal_color.red==0)
  {
    // moving towards pure blue from turquoise
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards pure blue from turquoise");
      Serial.println(line);
    }

    temporal_color.green--;
  }
  else if(temporal_color.blue==255 && temporal_color.red<255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards violet from pure blue");
      Serial.println(line);
    }

    temporal_color.red++;
  }
  else if(temporal_color.blue>0 && temporal_color.red==255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(debug)
    {
      char line[100];
      sprintf(line,"moving towards red from violet");
      Serial.println(line);
    }

    temporal_color.blue--;
  }
}

int comet_first_time=true;
CRGB upper_comet_pattern[NUM_LEDS_UPPER];
CRGB lower_comet_pattern[NUM_LEDS_LOWER];
CRGB comet_color;
CRGB bg_color;
int tail_length = 4;
int comet_movement_interval_ms = 1000ms;
int last_comet_movement_time_ms = millis();
int upper_comet_index = 0;
int lower_comet_index = 0;
void comet()
{
  if(comet_first_time)
  {
    comet_color.red = 255;
    comet_color.blue = 255;
    comet_color.green = 0;

    bg_color.red = 0;
    bg_color.green = 0;
    bg_color.blue = 255;

    if(use_upper_leds)
    {
      fill_solid(upper_comet_pattern,NUM_LEDS_UPPER,bg_color);
      int comet_color_change_amount = 255/tail_length;
      for(int t=0;t<tail_length;t++)
      {
        upper_comet_pattern[NUM_LEDS_UPPER-t] = comet_color;
        upper_comet_pattern[NUM_LEDS_UPPER-t].red -=t*comet_color_change_amount;
      }
    }
    if(use_lower_leds)
    {
      fill_solid(lower_comet_pattern,NUM_LEDS_LOWER,bg_color);
      int comet_color_change_amount = 255/tail_length;
      for(int t=0;t<tail_length;t++)
      {
        lower_comet_pattern[NUM_LEDS_LOWER-t] = comet_color;
        lower_comet_pattern[NUM_LEDS_LOWER-t].red -=t*comet_color_change_amount;
      }
    }
    comet_first_time=false;
  }


  // fill the actual lower and upper leds by overlaying the comet pattern but offsetting every comet movement interval
  int current_comet_movement_time_ms = millis();
  int time_diff = current_comet_movement_time_ms = last_comet_movement_time_ms;
  if(time_diff>comet_movement_interval_ms)
  {
    // overlay values and 
    if(use_upper_leds)
    {
      int source_index = 0;
      for(int ci=upper_comet_index;ci<NUM_LEDS_UPPER;ci++)
      {
        int real_index = ci%NUM_LEDS_UPPER;
        leds_upper[real_index] = upper_comet_pattern[source_index];
        source_index++
      }
      upper_comet_index++;
      if(upper_comet_index>=NUM_LEDS_UPPER)
      {
        upper_comet_index = 0;
      }
    }

    last_comet_movement_time_ms = current_comet_movement_time_ms;
  }
  
  

}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds_upper, NUM_LEDS_UPPER, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds_upper[ random16(NUM_LEDS_UPPER) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds_upper, NUM_LEDS_UPPER, 10);
  int pos = random16(NUM_LEDS_UPPER);
  leds_upper[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds_upper, NUM_LEDS_UPPER, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS_UPPER-1 );
  leds_upper[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS_UPPER; i++) { //9948
    leds_upper[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds_upper, NUM_LEDS_UPPER, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds_upper[beatsin16( i+7, 0, NUM_LEDS_UPPER-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

