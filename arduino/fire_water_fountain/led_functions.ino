//int current_led_program_id = AVS_COLORS;
//int current_led_program_id = NUGGETS_COLORS;
//int current_led_program_id = CUBS_COLORS;
//int current_led_program_id = BRONCOS_COLORS;

CRGB all_led[NUMBER_OF_LED_LIGHTS][NUMBER_LEDS_PER_LIGHT];

bool m_debug_led=false;

bool lastLEDProgramButton = LOW;
int current_led_program_id = 0;

void readLEDProgramButton()
{
  bool currentButton = digitalRead(LED_PROGRAM_TOGGLE);
  if(currentButton==HIGH && lastLEDProgramButton==LOW)
  {
    current_led_program_id = (current_led_program_id+1)%MAX_LED_PROGRAMS;
    Serial.print("led program = ");
    Serial.print(current_led_program_id);
    Serial.print("\n");
    lastLEDProgramButton = HIGH;
  }
  if(currentButton==LOW && lastLEDProgramButton==HIGH)
  {
    // reset
    lastLEDProgramButton=LOW;
  }
}


void setupLEDs()
{
  bool use_led_left=true;
  bool use_led_center=true;
  bool use_led_right=true;

  // this will setup pumps and do the proper pin assignments on the arduino
  if(!use_leds)
  {
    return;
  }
  
#if 0  
  Serial.print("in setupLEDs with ");
  Serial.print(NUMBER_OF_LED_LIGHTS);
  Serial.print(" led lights present");
  Serial.print("\n");
#endif
  
  for(int p=0;p<NUMBER_OF_LED_LIGHTS;p++)
  {    
    led_controller[p].setDebugLevel(debugLevel);

    char name[100];
#if 0 
    Serial.print(p);
    Serial.print(" ");
    Serial.print(use_led_left);
    Serial.print("\n");
#endif

    if(p==LEFT && use_led_left)
    {
      // left
      sprintf(name,"left_led");    
      for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
      {
        all_led[p][i].red=255;
        all_led[p][i].green=255;
        all_led[p][i].blue=255;      
      }
      led_controller[p].setupLED(name,NUMBER_LEDS_PER_LIGHT);
#if 0      
      Serial.print("setting up left led with ");
      Serial.print(NUMBER_LEDS_PER_LIGHT);
      Serial.print(" leds");
      Serial.print("\n");
#endif      
      FastLED.addLeds<LED_TYPE,LEFT_LED_CONTROL_PIN,COLOR_ORDER>(all_led[p], NUMBER_LEDS_PER_LIGHT).setCorrection(TypicalLEDStrip);
#if 0
      Serial.print("setup left led");
      Serial.print("\n");
#endif
    }
    else if(p==CENTER && use_led_center)
    {
      // center
      sprintf(name,"center_led");
      for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
      {
        all_led[p][i].red=255;
        all_led[p][i].green=255;
        all_led[p][i].blue=255;      
      }
      led_controller[p].setupLED(name,NUMBER_LEDS_PER_LIGHT);
      FastLED.addLeds<LED_TYPE,CENTER_LED_CONTROL_PIN,COLOR_ORDER>(all_led[p], NUMBER_LEDS_PER_LIGHT).setCorrection(TypicalLEDStrip);
#if 0
      Serial.print("setup center led");
      Serial.print("\n");
#endif      
    }
    else if(p==RIGHT && use_led_right)
    {
      // right
      sprintf(name,"right_led");
      for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
      {
        all_led[p][i].red=255;
        all_led[p][i].green=255;
        all_led[p][i].blue=255;
      }
      led_controller[p].setupLED(name,NUMBER_LEDS_PER_LIGHT);
      FastLED.addLeds<LED_TYPE,RIGHT_LED_CONTROL_PIN,COLOR_ORDER>(all_led[p], NUMBER_LEDS_PER_LIGHT).setCorrection(TypicalLEDStrip);
#if 0
      Serial.print("setup right led");
      Serial.print("\n");
#endif      
    }
  }

  FastLED.setBrightness(255);
  FastLED.show();
  FastLED.delay(10);
  delay(2000);
}

bool led_test()
{
  for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
  {
    all_led[LEFT][i].red = 255;
    all_led[LEFT][i].green=0;
    all_led[LEFT][i].blue=0;
  }

  for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
  {
    all_led[CENTER][i].red = 0;
    all_led[CENTER][i].green=0;
    all_led[CENTER][i].blue=255;
  }
  
  for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
  {
    all_led[RIGHT][i].red = 0;
    all_led[RIGHT][i].green=255;
    all_led[RIGHT][i].blue=0;
  }
  FastLED.show();
  FastLED.delay(10);
  return true; 
}


void runLEDProgram()
{ 
  if(current_led_program_id==LED_TEMPORAL_SPECTRUM)
  {
    temporalSpectrum();
  }
  else if(current_led_program_id==LED_RANDOM)
  {
    randomLED();
  }
  else if(current_led_program_id==LED_RANDOM_SYNCED)
  {
    randomLEDSynced();
  }
  else if(current_led_program_id==AVS_COLORS)
  {
    avsColors();
  }
  else if(current_led_program_id==NUGGETS_COLORS)
  {
    nuggetsColors();
  }
  else if(current_led_program_id==CUBS_COLORS)
  {
    cubsColors();
  }
  else if(current_led_program_id==BRONCOS_COLORS)
  {
    broncosColors();
  }      
  FastLED.show();
  FastLED.delay(10);
}

void avsColors()
{
  static int phase=1;
  CRGB burgundy;
  CRGB blue;
  burgundy.red=200;
  burgundy.green=38;
  burgundy.blue=61;
  blue.red= 35;
  blue.green=97;
//  blue.blue=146;
  blue.blue=200;


  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }

  if(phase==0)
  {
    phase=1;
  }
  else if(phase==1)
  {
    phase=0;
  }

  // if here, we are ready for the next color
  color_start_time_ms = millis();



  if(phase==0)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=blue;
      all_led[2][i]=blue;
      all_led[1][i]=burgundy;
    }
  }
  else if(phase==1)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=burgundy;
      all_led[2][i]=burgundy;
      all_led[1][i]=blue;
    }
  }  
}

void nuggetsColors()
{
  static int phase=1;
  CRGB yellow;
  CRGB blue;
  yellow.red=253;
  yellow.green=250;
  yellow.blue=0;
  blue.red= 80;
  blue.green=145;
//  blue.blue=146;
  blue.blue=250;


  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }

  if(phase==0)
  {
    phase=1;
  }
  else if(phase==1)
  {
    phase=0;
  }

  // if here, we are ready for the next color
  color_start_time_ms = millis();



  if(phase==0)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=blue;
      all_led[2][i]=blue;
      all_led[1][i]=yellow;
    }
  }
  else if(phase==1)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=yellow;
      all_led[2][i]=yellow;
      all_led[1][i]=blue;
    }
  }  
}

void cubsColors()
{
  static int phase=1;
  CRGB red;
  CRGB blue;
  red.red=254;
  red.green=0;
  red.blue=0;
  blue.red= 0;
  blue.green=0;
//  blue.blue=146;
  blue.blue=200;


  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }

  if(phase==0)
  {
    phase=1;
  }
  else if(phase==1)
  {
    phase=0;
  }

  // if here, we are ready for the next color
  color_start_time_ms = millis();



  if(phase==0)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=blue;
      all_led[2][i]=blue;
      all_led[1][i]=red;
    }
  }
  else if(phase==1)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=red;
      all_led[2][i]=red;
      all_led[1][i]=blue;
    }
  }  
}

void broncosColors()
{
  static int phase=1;
  CRGB orange;
  CRGB blue;
  orange.red=180;
  orange.green=60;
  orange.blue=10;
  blue.red= 0;
  blue.green=0;
//  blue.blue=146;
  blue.blue=80;


  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }

  if(phase==0)
  {
    phase=1;
  }
  else if(phase==1)
  {
    phase=0;
  }

  // if here, we are ready for the next color
  color_start_time_ms = millis();



  if(phase==0)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=blue;
      all_led[2][i]=blue;
      all_led[1][i]=orange;
    }
  }
  else if(phase==1)
  {
    // alternate between the columns
    // start out with two outer are blue and center is burgundy
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[0][i]=orange;
      all_led[2][i]=orange;
      all_led[1][i]=blue;
    }
  }  
}


void temporalSpectrum()
{
  float slow_factor = 0.2; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }


  // if here, we are ready for the next color
  color_start_time_ms = millis();

  static CRGB temporal_color(255,0,0);

  if(temporal_color.red==255 && temporal_color.green<255 && temporal_color.blue==0)
  {
    // moving towards yellow from red
    if(m_debug_led)
    {
 //     Serial.print("moving towards yellow from red");
    } 

    temporal_color.green++;
  }
  else if(temporal_color.green==255 && temporal_color.red>0 && temporal_color.blue==0)
  {
    // moving towards pure green from yellow
    if(m_debug_led)
    {
//      Serial.print("moving towards pure green from yellow");
    }

    temporal_color.red--;
  }
  else if(temporal_color.green==255 && temporal_color.red==0 && temporal_color.blue<255)
  {
    //moving towards turquoise from green
    if(m_debug_led)
    {
//        Serial.print("moving towards turquoise from green");
    }

    temporal_color.blue++;
  }
  else if(temporal_color.blue==255 && temporal_color.green>0 && temporal_color.red==0)
  {
    // moving towards pure blue from turquoise
    if(m_debug_led)
    {
 //     Serial.println("moving towards pure blue from turquoise");
    }

    temporal_color.green--;
  }
  else if(temporal_color.blue==255 && temporal_color.red<255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(m_debug_led)
    {
     Serial.print("moving towards violet from pure blue");
    }

    temporal_color.red++;
  }
  else if(temporal_color.blue>0 && temporal_color.red==255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(m_debug_led)
    {
 //      Serial.print("moving towards red from violet");
    }

    temporal_color.blue--;
  }

  // fill the led arrays
  for(int l=0;l<NUMBER_OF_LED_LIGHTS;l++)
  {
    if(!led_controller[l].m_use)
    {
      continue;
    }
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[l][i].red = temporal_color.red;
      all_led[l][i].green = temporal_color.green;
      all_led[l][i].blue = temporal_color.blue;
    }
  }
} 

void randomLED()
{
  float slow_factor = 0.2; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }


  // if here, we are ready for the next color
  color_start_time_ms = millis();

  // fill the led arrays
  for(int l=0;l<NUMBER_OF_LED_LIGHTS;l++)
  {
    if(!led_controller[l].m_use)
    {
      continue;
    }

    int random_red = random(0, 255);
    int random_green = random(0, 255);
    int random_blue = random(0, 255);

    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[l][i].red = random_red;
      all_led[l][i].green = random_green;
      all_led[l][i].blue = random_blue;
    }
  }
}

void randomLEDSynced()
{
  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long color_start_time_ms = 0;
  if(first_time)
  {
    color_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - color_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  if(!first_time)
  {
    if(elapsed_time_ms<time_threshold_ms)
    {
      return;
    }
  }
  else
  {
    first_time=false;
  }


  // if here, we are ready for the next color
  color_start_time_ms = millis();

  // fill the led arrays
  int random_red = random(0, 255);
  int random_green = random(0, 255);
  int random_blue = random(0, 255);
  for(int l=0;l<NUMBER_OF_LED_LIGHTS;l++)
  {
    if(!led_controller[l].m_use)
    {
      continue;
    }

 
    for(int i=0;i<NUMBER_LEDS_PER_LIGHT;i++)
    {
      all_led[l][i].red = random_red;
      all_led[l][i].green = random_green;
      all_led[l][i].blue = random_blue;
    }
  }
}
