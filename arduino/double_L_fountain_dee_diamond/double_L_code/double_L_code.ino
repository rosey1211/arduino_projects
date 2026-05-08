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

// in this case, UPPER is the left inverted L since it is actually above the other cross-member
// LOWER is the right inverted L
const int LED=7;
const int BUTTON=2;
#define UPPER_DATA_PIN    5
#define LOWER_DATA_PIN    4

//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

//#define UPPER_NUM_LEDS 20
#define UPPER_NUM_LEDS    49
CRGB upper_leds[UPPER_NUM_LEDS];

//#define LOWER_NUM_LEDS    20
#define LOWER_NUM_LEDS    49
CRGB lower_leds[LOWER_NUM_LEDS];
// this is used for the opposing version of the comet
CRGB temp_led_array[LOWER_NUM_LEDS];

#define MAX_BRIGHTNESS     100
#define FRAMES_PER_SECOND  120

bool use_upper_led_strip = true;
bool use_lower_led_strip = true;

bool use_temporal_rainbow=false;
bool use_comet=true;

// comet setup stuff
CRGB upper_initial_comet[UPPER_NUM_LEDS];
CRGB lower_initial_comet[LOWER_NUM_LEDS];
int comet_percent=0;

// for potentiometer
const int BRIGHTNESS_POT=0; // pot on analog pin 0
const int TIME_PERIOD_POT=1; // pot on analog pin 1
int brightness_pot_val = 0;
int time_period_pot_val = 0.5*1023;

double max_pot_value = 1023.0;
double min_pot_value = 0.0;

// toggle button stuff - mostly for debouncing
//bool lastButton = LOW;
//bool currentButton = LOW;
// current led routine
int pattern_id = 0;
// for now, must temporal rainbow and comet.  As more are added, this number will increase
int max_num_patterns = 4;

#define CHECK_POTS_INTERVAL 100

bool debug = false;
bool superDebug = false;
bool insaneSuperDebug = true;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // setup the button
  pinMode(BUTTON,INPUT);
  pinMode(LED,OUTPUT);

  // tell FastLED about the LED strip configuration
  if(use_upper_led_strip)
  {
    FastLED.addLeds<LED_TYPE,UPPER_DATA_PIN,COLOR_ORDER>(upper_leds, UPPER_NUM_LEDS).setCorrection(TypicalLEDStrip);
  }

  if(use_lower_led_strip)
  {
    FastLED.addLeds<LED_TYPE,LOWER_DATA_PIN,COLOR_ORDER>(lower_leds, LOWER_NUM_LEDS).setCorrection(TypicalLEDStrip);
  }

  // initialize comet if used
  if(use_comet)
  {
    if(use_upper_led_strip)
    {
      if(insaneSuperDebug)
      {
        char line[100];
        sprintf(line,"Initializing upper comet");
        Serial.println(line);
      }
      initializeComet(upper_initial_comet,UPPER_NUM_LEDS);
    }
    if(use_lower_led_strip)
    {
      initializeComet(lower_initial_comet,LOWER_NUM_LEDS);
    }
  }
  
  
  // initialize random number generator using unconnected analog pin 1
  randomSeed(analogRead(1));

  // set master brightness control
  FastLED.setBrightness(MAX_BRIGHTNESS);

  Serial.begin(9600);
}


int max_time_period_ms=30*1000/FRAMES_PER_SECOND;
int min_time_period_ms=1000/FRAMES_PER_SECOND;
int time_delay = min_time_period_ms;

int last_loop_time_ms = millis();
int read_pot_interval_ms = 2000; // every 2 seconds

// for randomFill function
int random_fill_interval = 5;
int random_fill_count=0;
// pattern_id values - new patterns will get added in here
enum
{
  SYNCED_COMET,
  OPPOSING_COMET,
  TEMPORAL_RAINBOW,
  RANDOM_FILL
};

// debounce routine
bool debounce()
{
  bool current = digitalRead(BUTTON);
  if(current==HIGH)
  {
    // read again to make sure the current is still high
    delay(100); // delay 5 ms
    current = digitalRead(BUTTON);
    if(current==HIGH)
    {
      return true;
    }
  } 
  return false;
}

bool lastButton = LOW;
void loop()
{ 

#if 1  
  bool currentButton = digitalRead(BUTTON);
   
  if(currentButton==HIGH && lastButton==LOW)
  {
      char line[100];
      digitalWrite(LED,HIGH);
      pattern_id=(pattern_id+1)%max_num_patterns; 
  }
  else if(currentButton==LOW)
  {
      digitalWrite(LED,LOW);
  }

  lastButton=currentButton;


  if(insaneSuperDebug)
  {
      char line[100];
      sprintf(line,"pattern_id = %d",pattern_id);
      Serial.println(line);
  }
  //return;
#endif
  if(insaneSuperDebug)
  {
      char line[100];
      sprintf(line,"cycling",pattern_id);
      Serial.println(line);

  }
  if(pattern_id==TEMPORAL_RAINBOW)
  {
    adjustTemporalRainbowColor();
    if(use_upper_led_strip)
    {
     sendColorToSpecificLEDStrip(upper_leds,UPPER_NUM_LEDS);  
    }

    if(use_lower_led_strip)
    {
       sendColorToSpecificLEDStrip(lower_leds,LOWER_NUM_LEDS);  
    }
   }
  else if(pattern_id==SYNCED_COMET)
  {
    if(use_upper_led_strip)
    {
     adjustComet(upper_initial_comet,upper_leds,UPPER_NUM_LEDS);
    }
    
    if(use_lower_led_strip)
    {
      adjustComet(lower_initial_comet,lower_leds,LOWER_NUM_LEDS);
    }
    comet_percent = (comet_percent+1)%100;

  }
  else if(pattern_id==OPPOSING_COMET)
  {
    if(use_upper_led_strip)
    {
     adjustComet(upper_initial_comet,upper_leds,UPPER_NUM_LEDS);
    }
    
    if(use_lower_led_strip)
    {
      invertedAdjustComet(lower_initial_comet,lower_leds,LOWER_NUM_LEDS);
    }
    
    comet_percent = (comet_percent+1)%100;
  }
  else if(pattern_id==RANDOM_FILL)
  {
    int result = random_fill_count%random_fill_interval;
    if(result==0)
    {
      if(use_upper_led_strip)
      {
       randomFill(upper_initial_comet,upper_leds,UPPER_NUM_LEDS);
      }
    
      if(use_lower_led_strip)
      {
        randomFill(lower_initial_comet,lower_leds,LOWER_NUM_LEDS);
      }
    }

    random_fill_count++;
    if(random_fill_count>random_fill_interval)
    {
      random_fill_count=0;
    }
  }
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //'FastLED.delay(1000/FRAMES_PER_SECOND); 
  FastLED.delay(time_delay); 
    
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


CRGB temporal_color(255,0,0);
void sendColorToSpecificLEDStrip(CRGB *led_array,int num_leds)  
{
  fill_solid( led_array, num_leds,temporal_color);
}

void adjustTemporalRainbowColor()
{
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
// ranges from 0-100 and then raps around to zero again
CRGB comet_tail_color(20,8,80);
void initializeComet(CRGB *initial_comet_array, int num_leds)
{
  // first make the array all light blue

  // then add some indigo, white and indigo
  fill_solid(initial_comet_array, num_leds, comet_tail_color);

   // now build the comet part
  // indigo
 
  initial_comet_array[0].red = 255;
  initial_comet_array[0].green = 0;
  initial_comet_array[0].blue = 200;

  initial_comet_array[1].red = 255;
  initial_comet_array[1].green = 0;
  initial_comet_array[1].blue = 200;

  // whiter indigo
  initial_comet_array[2].red = 255;
  initial_comet_array[2].green = 100;
  initial_comet_array[2].blue = 200;

  initial_comet_array[3].red = 255;
  initial_comet_array[3].green = 100;
  initial_comet_array[3].blue = 200;

  initial_comet_array[4].red = 255;
  initial_comet_array[4].green = 100;
  initial_comet_array[4].blue = 200;
  // white
  initial_comet_array[5].red = 255;
  initial_comet_array[5].green = 255;
  initial_comet_array[5].blue = 255;
  
  initial_comet_array[6].red = 255;
  initial_comet_array[6].green = 255;
  initial_comet_array[6].blue = 255;
  
  initial_comet_array[7].red = 255;
  initial_comet_array[7].green = 255;
  initial_comet_array[7].blue = 255;

  initial_comet_array[8].red = 255;
  initial_comet_array[8].green = 255;
  initial_comet_array[8].blue = 255;
} 




void adjustComet(CRGB *initial_array, CRGB *led_array, int num_leds)
{
  // figure out the comet head index
  double real_percent = (double)comet_percent/100.0;
  int start_index = (int)(0.5+real_percent*(double)num_leds);
  for(int i=0;i<num_leds;i++)
  {
    int target_index = (start_index+i)%num_leds;
    led_array[target_index] = initial_array[i];
  }
 
  //sprintf(line,"start_index=%d : comet_percent=%d\n",start_index,comet_percent);
  //Serial.println(line);
}

void invertedAdjustComet(CRGB *initial_array, CRGB *led_array, int num_leds)
{
  // figure out the comet head index
  double real_percent = (double)comet_percent/100.0;
  int start_index = (int)(0.5+real_percent*(double)num_leds);
  for(int i=0;i<num_leds;i++)
  {
    int target_index = (start_index+i)%num_leds;
    temp_led_array[target_index] = initial_array[i];
  }

  // now just invert
  for(int i=0;i<num_leds;i++)
  {
    int inv_i = num_leds - i - 1;
    led_array[i] = temp_led_array[inv_i];
  }

  //sprintf(line,"start_index=%d : comet_percent=%d\n",start_index,comet_percent);
  //Serial.println(line);
}

void randomFill(CRGB *initial_array, CRGB *led_array, int num_leds)
{
  // random locations with a single random color
  // first set the entire array to black
  for(int i=0;i<num_leds;i++)
  {
    led_array[i].red = 0;
    led_array[i].green = 0;
    led_array[i].blue = 0;
  }

  // set the color
  // get a red value
  int red = random(0,255);
  int green = random(0,255);
  int blue = random(0,255);

  CRGB new_color;
  new_color.red = red;
  new_color.green = green;
  new_color.blue = blue;

  // now fill in random locations
  double percent_to_fill = 0.25;
  for(int i=0;i<percent_to_fill*num_leds;i++)
  {
    int loc = random(0,num_leds-1);
    led_array[loc] = new_color;
  }
  
}
