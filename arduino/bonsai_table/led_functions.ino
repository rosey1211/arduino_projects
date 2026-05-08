


CRGB temporal_color(255,0,0);

void adjustColorBasedOnBrightness(CRGB input_color, CRGB &output_color)
{
   float scaled_brightness = (float)currentBrightnessLevel/(float)maxBrightnessLevel;
   int new_red;
   int new_green;
   int new_blue;
   

   if(input_color.red>=input_color.green && input_color.red>=input_color.blue)
   {
	// red is max
	// compute ratios
	float green_to_red_ratio = (float)input_color.green/(float)input_color.red;
	float blue_to_red_ratio = (float)input_color.blue/(float)input_color.red;

        new_red = scaled_brightness*input_color.red;
	new_green = (int)(0.5+(float)new_red*green_to_red_ratio);
	new_blue = (int)(0.5+(float)new_red*blue_to_red_ratio);	
   }
   else if(input_color.green>=input_color.red && input_color.green>=input_color.blue)
   {
	// green is max
	// compute ratios
	float red_to_green_ratio = (float)input_color.red/(float)input_color.green;
	float blue_to_green_ratio = (float)input_color.blue/(float)input_color.green;

        new_green = scaled_brightness*input_color.green;
	new_red = (int)(0.5+(float)new_green*red_to_green_ratio);
	new_blue = (int)(0.5+(float)new_green*blue_to_green_ratio);	
   }   
   else if(input_color.blue>=input_color.red && input_color.blue>=input_color.green)
   {
	// blue is max
	// compute ratios
	float red_to_blue_ratio = (float)input_color.red/(float)input_color.blue;
	float green_to_blue_ratio = (float)input_color.green/(float)input_color.blue;

        new_blue = scaled_brightness*input_color.blue;
	new_red = (int)(0.5+(float)new_blue*red_to_blue_ratio);
	new_green = (int)(0.5+(float)new_blue*green_to_blue_ratio);	
   }   

   output_color.red = new_red;
   output_color.green = new_green;
   output_color.blue = new_blue;
#if 0
   if(1)
   {
	Serial.print("pot brightness level = ");
	Serial.print(currentBrightnessLevel);
	Serial.print(" : scaled_brightness = ");
	Serial.print(scaled_brightness);

	Serial.print(" : pot color level = ");
	Serial.print(currentColorLevel);

        Serial.print(": in_red = ");
	Serial.print(input_color.red);
	Serial.print(": in_geen = ");
	Serial.print(input_color.green);
	Serial.print(": in_blue = ");
	Serial.print(input_color.blue);

	Serial.print(": out_red = ");
	Serial.print(output_color.red);
	Serial.print(": out_geen = ");
	Serial.print(output_color.green);
	Serial.print(": out_blue = ");
	Serial.print(output_color.blue);
	Serial.print("\n");
   }
#endif
}

void setupColorRange()
{
   no_color.red=0;
   no_color.green=0;
   no_color.blue=0;
   
   colorSample = (CRGB *)malloc(sizeof(CRGB)*6*256/color_skip);
   int color_index=0;
   // start increasing green
   // moving to yellow from red
   for(int g=0;g<=255;g+=color_skip)
   {
      colorSample[color_index].red = 255;
      colorSample[color_index].green=g;
      colorSample[color_index].blue=0;
      color_index++;
   }

   // start decreasing red
   // moving from yellow to green
   for(int r=color_skip;r<=255;r+=color_skip)
   {
      colorSample[color_index].red = 255-r;
      colorSample[color_index].green=255;
      colorSample[color_index].blue=0;
      color_index++;
   }

   // start increasing blue
   // start moving to turquoise from green
   for(int b=color_skip;b<=255;b+=color_skip)
   {
      colorSample[color_index].red = 0;
      colorSample[color_index].green=255;
      colorSample[color_index].blue=b;
      color_index++;
   }

   // start decreasing green
   // moving from turquoise to blue
   for(int g=color_skip;g<=255;g+=color_skip)
   {
      colorSample[color_index].red = 0;
      colorSample[color_index].green=255-g;
      colorSample[color_index].blue=255;
      color_index++;
   }

   // start increasing red
   // moving from blue to violet
   for(int r=color_skip;r<=255;r+=color_skip)
   {
      colorSample[color_index].red = r;
      colorSample[color_index].green=0;
      colorSample[color_index].blue=255;
      color_index++;
   }

   // start decreasing blue
   // moving from violet to red
   for(int b=color_skip;b<=255;b+=color_skip)
   {
      colorSample[color_index].red = 255;
      colorSample[color_index].green=0;
      colorSample[color_index].blue=255-b;
      color_index++;
   }

   total_number_of_colors = color_index;

   if(debugLevel>2)
   {
      for(int i=0;i<color_index;i++)
      {
	Serial.print("colorSample[");
	Serial.print(i);
	Serial.print("] : ");
	Serial.print("red = ");
	Serial.print(colorSample[i].red);
	Serial.print(" : green = ");
	Serial.print(colorSample[i].green);
	Serial.print(" : blue = ");
	Serial.print(colorSample[i].blue);
	Serial.print("\n");
      }
   }


}

void singleColor()
{
   // get the color pot value and compute the index into the colorSamples
   int local_color_index = (int)(0.5 + (float)total_number_of_colors*(float)currentColorLevel/(float)maxColorLevel);
   CRGB currentColor = colorSample[local_color_index];
   CRGB adjustedColor;
   adjustColorBasedOnBrightness(currentColor,adjustedColor);
   for(int i=0;i<NUM_LEDS;i++)
   {
      leds[i] = adjustedColor;
   }
}

void rotatingColor()
{
  static int current_led_block=0;
  
  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 1000.0;
  static bool first_time=true;
  static long led_block_start_time_ms = 0;
  if(first_time)
  {
    led_block_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - led_block_start_time_ms;
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
  led_block_start_time_ms = millis();
  current_led_block = (current_led_block+1)%NUM_LED_BLOCKS;
  
  // get the color pot value and compute the index into the colorSamples
  int local_color_index = (int)(0.5 + (float)total_number_of_colors*(float)currentColorLevel/(float)maxColorLevel);
  CRGB currentColor = colorSample[local_color_index];
  CRGB adjustedColor;
  adjustColorBasedOnBrightness(currentColor,adjustedColor);

  // now alter which of the two led blocks gets turned on
  // there are four blocks, and just rotate
  for(int i=0;i<NUM_LEDS;i++)
   {
      leds[i] = no_color;
   }

   int num_leds_per_block[NUM_LED_BLOCKS];
   num_leds_per_block[0]=2;
   num_leds_per_block[1]=2;
   num_leds_per_block[2]=2;
   num_leds_per_block[3]=2;

   int led_block_start_index[NUM_LED_BLOCKS];
   int block_sum=0;
   for(int i=0;i<NUM_LED_BLOCKS;i++)
   {
      led_block_start_index[i] = block_sum;
      block_sum+=num_leds_per_block[i];
   }

   for(int i=led_block_start_index[current_led_block];i<led_block_start_index[current_led_block]+num_leds_per_block[current_led_block];i++)
   {
	leds[i] = adjustedColor;
	if(0)
        {
	   Serial.print("current_led_block = ");
	   Serial.print(current_led_block);
	   Serial.print(" : led_index = ");
	   Serial.print(i);
	   Serial.print("\n");
        }
   }

}

void temporalRainbow()
{
  float slow_factor = 0.2; // between [0,1]
  float max_delay_ms = 100.0;
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

  static CRGB temporal_color(0,255,0);

  // note: with the ws2815, the color ordering is a little wacked.
  // The middle value green is really red, the first value is green and the last is blue

  if(temporal_color.red==255 && temporal_color.green<255 && temporal_color.blue==0)
  {
    // moving towards yellow from red
    if(debugLevel>1)
    {
      Serial.print("moving towards yellow from red");
      Serial.print("\n");
    } 

    temporal_color.green++;
  }
  else if(temporal_color.green==255 && temporal_color.red>0 && temporal_color.blue==0)
  {
    // moving towards pure green from yellow
    if(debugLevel>1)
    {
      Serial.print("moving towards pure green from yellow");
      Serial.print("\n");      
    }

    temporal_color.red--;
  }
  else if(temporal_color.green==255 && temporal_color.red==0 && temporal_color.blue<255)
  {
    //moving towards turquoise from green
    if(debugLevel>1)
    {
        Serial.print("moving towards turquoise from green");
	Serial.print("\n");
    }

    temporal_color.blue++;
  }
  else if(temporal_color.blue==255 && temporal_color.green>0 && temporal_color.red==0)
  {
    // moving towards pure blue from turquoise
    if(debugLevel>1)
    {
     Serial.println("moving towards pure blue from turquoise");
     Serial.print("\n");
    }

    temporal_color.green--;
  }
  else if(temporal_color.blue==255 && temporal_color.red<255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(debugLevel>1)
    {
     Serial.print("moving towards violet from pure blue");
     Serial.print("\n");
     }

    temporal_color.red++;
  }
  else if(temporal_color.blue>0 && temporal_color.red==255 && temporal_color.green==0)
  {
    // moving towards violet from pure blue
    if(debugLevel>1)
    {
      Serial.print("moving towards red from violet");
      Serial.print("\n");
    }

    temporal_color.blue--;
  }

  CRGB adjusted_color;
  adjustColorBasedOnBrightness(temporal_color,adjusted_color);


  // fill the led arrays
  for(int i=0;i<NUM_LEDS;i++)
    {
      leds[i].red = adjusted_color.red;
      leds[i].green = adjusted_color.green;
      leds[i].blue = adjusted_color.blue;
    }
}



void readLEDProgramButton()
{
  bool currentButton = digitalRead(LED_PROGRAM_TOGGLE_PIN);
  if(currentButton==HIGH && lastLEDProgramButton==LOW)
  {
    currentLEDProgramID = (currentLEDProgramID+1)%MAX_NUMBER_LED_PATTERNS;
    Serial.print("led program = ");
    Serial.print(currentLEDProgramID);
    Serial.print("\n");
    lastLEDProgramButton = HIGH;
  }
  if(currentButton==LOW && lastLEDProgramButton==HIGH)
  {
    // reset
    lastLEDProgramButton=LOW;
  }
}


void runLEDProgram()
{ 
  if(currentLEDProgramID==SINGLE_COLOR)
  {
    singleColor();
  }
#if 0
  else if(currentLEDProgramID==ROTATING_COLOR)
  {
    rotatingColor();
  }
#endif  
  else if(currentLEDProgramID==TEMPORAL_RAINBOW)
  {
    temporalRainbow();
  }

  FastLED.show();
  FastLED.delay(10);
}
