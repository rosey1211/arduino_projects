


#if defined (REAL)
void setupLEDBlocks()
{
  // do for future random numbers in the future
  //randomSeed(analogRead(0));
  randomSeed(millis());
  if(g_percentage_active_led_blocks>1.0) g_percentage_active_led_blocks=1.0;
  g_target_number_active_led_blocks = (int)(0.5 + g_percentage_active_led_blocks*(float)NUM_LED_BLOCKS);


  // setup led blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_active_led_block_list[i] = false;
  }

#if 0
  Serial.print("num_led_blocks = ");
  Serial.print(NUM_LED_BLOCKS);
  Serial.print(" : num_leds = ");
  Serial.print(NUM_LEDS);
  Serial.print("\n");
#endif

  unsigned int start_led=0;
  unsigned int end_led=0;
  unsigned int num_block_leds = 0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
     g_active_led_block_list[i] = false;


    if(i==0)
    {
      num_block_leds=3;
    }
    else if(i==1)
    {
      num_block_leds=5;
    }
    else if(i==2)
    {
      num_block_leds=4;
    }
    else if(i==3)
    {
      num_block_leds=6;
    }
    else if(i==4)
    {
      num_block_leds=7;
    }
    else if(i==5)
    {
      num_block_leds=5;
    }
    else if(i==6)
    {
      num_block_leds=7;
    }
    else if(i==7)
    {
      num_block_leds=8;
    }
    else if(i==8)
    {
      num_block_leds=7;
    }
    else if(i==9)
    {
      num_block_leds=5;
    }
    else if(i==10)
    {
      num_block_leds=7;
    }
    else if(i==11)
    {
      num_block_leds=4;
    }
    else if(i==12)
    {
      num_block_leds=4;
    }
    else if(i==13)
    {
      num_block_leds=3;
    }
    else if(i==14)
    {
      num_block_leds=5;
    }
    else if(i==15)
    {
      num_block_leds=4;
    }

    end_led=start_led+num_block_leds-1;
    g_led_block[i].m_startLED = start_led;
    g_led_block[i].m_endLED = end_led;
    g_led_block[i].m_blockID = i;
    g_led_block[i].m_maxTimeAtBrightnessLevelMS = g_maxTimeAtBrightnessLevelMS;
    g_led_block[i].m_brightnessChangeAmount = g_brightnessChangeAmount;
    g_led_block[i].m_active = false;
    g_led_block[i].m_debugLevel=0;
    g_led_block[i].m_settingID=0;
    g_led_block[i].m_mode=500;

    start_led = end_led+1;
  }

  reset_inactive_led_block_list();


  if(0)
  {
    Serial.print("target number of active blocks = ");
    Serial.print(g_target_number_active_led_blocks);
    Serial.print("\n");
    Serial.print("Test\n");
    g_active_led_block_list[0]=true;
    g_active_led_block_list[1]=false;
    g_active_led_block_list[2]=true;
    reset_inactive_led_block_list();
  }
}
#else
void setupLEDBlocks()
{
  // do for future random numbers in the future
  //randomSeed(analogRead(0));
  randomSeed(millis());

  if(g_percentage_active_led_blocks>1.0) g_percentage_active_led_blocks=1.0;
  g_target_number_active_led_blocks = (int)(0.5 + g_percentage_active_led_blocks*(float)NUM_LED_BLOCKS);

  // setup led blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_active_led_block_list[i] = false;
  }
  
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_active_led_block_list[i] = false;

    int start_led;
    int end_led;
    if(i==0)
    {
      start_led=0;
      end_led=1;
    }
    else if(i==1)
    {
      start_led=2;
      end_led=3;
    }
    else if(i==2)
    {
      start_led=4;
      end_led=5;
    }
    g_led_block[i].setup(start_led,end_led,i,g_maxTimeAtBrightnessLevelMS,g_brightnessChangeAmount);
  }
  
  reset_inactive_led_block_list();


  if(0)
  {
    Serial.print("target number of active blocks = ");
    Serial.print(g_target_number_active_led_blocks);
    Serial.print("\n");
    Serial.print("Test\n");
    g_active_led_block_list[0]=true;
    g_active_led_block_list[1]=false;
    g_active_led_block_list[2]=true;
    reset_inactive_led_block_list();
  }


}
#endif


void reset_inactive_led_block_list()
{
  // reset
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_just_inactive_led_block_list[i]=-1;
  }


  int inactive_count=0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(!g_active_led_block_list[i])
    {
      inactive_count++;
    }
  }
  if(inactive_count==0)
  {
    return;
  }

  int inactive_index=0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(!g_active_led_block_list[i])
    {
      g_just_inactive_led_block_list[inactive_index] = i;
      inactive_index++;
    }
  }  
  g_num_inactive_led_blocks = inactive_count;

  if(debugLevel>0)
  {
    Serial.print("\n\n");
    Serial.print("active block list\n");
    for(int i=0;i<NUM_LED_BLOCKS;i++)
    {
     Serial.print(g_active_led_block_list[i]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }


  if(g_num_inactive_led_blocks > 0)
  {
    // print the inactive list
    if(debugLevel>0)
    {
      Serial.print("Inactive List\n");
      for(int i=0;i<g_num_inactive_led_blocks;i++)
      {
       Serial.print(g_just_inactive_led_block_list[i]);
       Serial.print(" ");
      }
      Serial.print("\n\n");
    }
  }
}

void sameColorRandomBlocks()
{
  // figure out how many active led blocks there are
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(debugLevel>0)
    {
      Serial.print("led_block[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(g_led_block[i].m_active);
      Serial.print(" : ");
    }
    if(g_led_block[i].m_active)
    {
      g_active_led_block_list[i] = true;
    }
    else
    {
      g_active_led_block_list[i] = false;
    }
  }
  if(debugLevel>0)
    Serial.print("\n");

  int active_led_block_count=0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(g_active_led_block_list[i])
    {
      active_led_block_count++;
    }
  }

  int block_shortage = g_target_number_active_led_blocks - active_led_block_count;
  if(debugLevel>0)
  {
    Serial.print("active_led_block_count = ");
    Serial.print(active_led_block_count);
    Serial.print(" : target_led_block_count = ");
    Serial.print(g_target_number_active_led_blocks);
    Serial.print(" : block shortage = ");
    Serial.print(block_shortage);
    Serial.print("\n");
  }

  if(block_shortage>0)
  {
    // figure out what blocks to turn on by randomly selecting from the inactive list
    reset_inactive_led_block_list();

    if(g_num_inactive_led_blocks==0)
    {
        // no more blocks can be turned on - this seems like a problem
        Serial.print("Error: cannot turn on any more LED blocks");
        return;
    }

    // randomly select from this list by the shortage of active led blocks
    for(int i=0;i<block_shortage;i++)
    {
      // add 1 because function does max-1
      long new_index = random(0,g_num_inactive_led_blocks);
      if(debugLevel>0)
      {
        Serial.print("new_index = ");
        Serial.print(new_index);
        Serial.print("\n");
      }

      // get the block to make active
      int block_id = g_just_inactive_led_block_list[new_index];

      if(debugLevel>0)
      {
        Serial.print("choosing block ");
        Serial.print(block_id);
        Serial.print(" to activate\n");
      }

      // get the random color from the color list
      // actually this uses the color specified by the color pot
      // get the random brightness 
      // get the random mode
      // LEFT off here 10/12/2025
      //g_led_block[block_id].reinitialize(red, int green, int blue, float brightness_level, int mode);
      int random_color_index;
      float random_brightness_level;
      float random_brightness_change_rate;
      unsigned int random_mode;
      getRandomBlockSettings(random_color_index,random_brightness_level,random_brightness_change_rate,random_mode);

      // for this mode, use the color setting from the color pot
      int local_color_index = (int)(0.5 + (float)g_total_number_of_colors*(float)g_currentColorLevel/(float)g_maxColorLevel);
      uint8_t red = g_colorSample[local_color_index].red;
      uint8_t green = g_colorSample[local_color_index].green;
      uint8_t blue = g_colorSample[local_color_index].blue;
      reinitializeBlock(block_id,red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

      if(debugLevel>3)
      {
        Serial.print(" : r_bl = ");
        Serial.print(random_brightness_level);
        Serial.print(" : random_brightness_change_rate = ");
        Serial.print(random_brightness_change_rate);
        Serial.print(" : random_mode = ");
        Serial.print(random_mode);
        Serial.print("\n\n");
      }
    }

    if(debugLevel>0)
    {
      Serial.print("After resetting blocks\n");
      for(int i=0;i<NUM_LED_BLOCKS;i++)
      {
        Serial.print("led_block[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.print(g_led_block[i].m_active);
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    runOnceBlock(i);
    //g_led_block[i].runOnce(g_leds);
  }

}

void twoColorRandomBlocks(CRGB &color1,CRGB &color2)
{
  CRGB color[2];
  
  color[0].red = color1.red;
  color[0].green = color1.green;
  color[0].blue = color1.blue;

  color[1].red = color2.red;
  color[1].green = color2.green;
  color[1].blue = color2.blue;


  // figure out how many active led blocks there are
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(debugLevel>0)
    {
      Serial.print("led_block[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(g_led_block[i].m_active);
      Serial.print(" : ");
    }
    if(g_led_block[i].m_active)
    {
      g_active_led_block_list[i] = true;
    }
    else
    {
      g_active_led_block_list[i] = false;
    }
  }
  if(debugLevel>0)
    Serial.print("\n");

  int active_led_block_count=0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(g_active_led_block_list[i])
    {
      active_led_block_count++;
    }
  }

  int block_shortage = g_target_number_active_led_blocks - active_led_block_count;
  if(debugLevel>0)
  {
    Serial.print("active_led_block_count = ");
    Serial.print(active_led_block_count);
    Serial.print(" : target_led_block_count = ");
    Serial.print(g_target_number_active_led_blocks);
    Serial.print(" : block shortage = ");
    Serial.print(block_shortage);
    Serial.print("\n");
  }

  if(block_shortage>0)
  {
    // figure out what blocks to turn on by randomly selecting from the inactive list
    reset_inactive_led_block_list();

    if(g_num_inactive_led_blocks==0)
    {
        // no more blocks can be turned on - this seems like a problem
        Serial.print("Error: cannot turn on any more LED blocks");
        return;
    }

    // randomly select from this list by the shortage of active led blocks
    for(int i=0;i<block_shortage;i++)
    {
      // add 1 because function does max-1
      long new_index = random(0,g_num_inactive_led_blocks);
      if(debugLevel>0)
      {
        Serial.print("new_index = ");
        Serial.print(new_index);
        Serial.print("\n");
      }

      // get the block to make active
      int block_id = g_just_inactive_led_block_list[new_index];

      if(debugLevel>0)
      {
        Serial.print("choosing block ");
        Serial.print(block_id);
        Serial.print(" to activate\n");
      }

      // get the random color from the color list
      // actually this uses the color specified by the color pot
      // get the random brightness 
      // get the random mode
      // LEFT off here 10/12/2025
      //g_led_block[block_id].reinitialize(red, int green, int blue, float brightness_level, int mode);
      int random_color_index;
      float random_brightness_level;
      float random_brightness_change_rate;
      unsigned int random_mode;
      getRandomBlockSettings(random_color_index,random_brightness_level,random_brightness_change_rate,random_mode);

      // get a new random color index between 0 and 1
      long random_color_seed = random(0,100);
      if(random_color_seed>50)
        random_color_index=1;
      else
        random_color_index=0;

      if(debugLevel>3)
      {
        Serial.print("block ");
        Serial.print(block_id);
        Serial.print(": random_color_index = ");
        Serial.print(random_color_index);
        Serial.print(" : random_brightness = ");
        Serial.print(random_brightness_level);
        Serial.print("\n");
      }

      // for this mode, use the color setting from the color pot
      uint8_t red = color[random_color_index].red;
      uint8_t green = color[random_color_index].green;
      uint8_t blue = color[random_color_index].blue;

      reinitializeBlock(block_id,red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

      if(debugLevel>3)
      {
        Serial.print(" : r_bl = ");
        Serial.print(random_brightness_level);
        Serial.print(" : random_brightness_change_rate = ");
        Serial.print(random_brightness_change_rate);
        Serial.print(" : random_mode = ");
        Serial.print(random_mode);
        Serial.print("\n\n");
      }
    }

    if(debugLevel>0)
    {
      Serial.print("After resetting blocks\n");
      for(int i=0;i<NUM_LED_BLOCKS;i++)
      {
        Serial.print("led_block[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.print(g_led_block[i].m_active);
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    runOnceBlock(i);
  }

}


void differentColorRandomBlocks()
{
  // figure out how many active led blocks there are
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(debugLevel>0)
    {
      Serial.print("led_block[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(g_led_block[i].m_active);
      Serial.print(" : ");
    }
    if(g_led_block[i].m_active)
    {
      g_active_led_block_list[i] = true;
    }
    else
    {
      g_active_led_block_list[i] = false;
    }
  }
  if(debugLevel>0)
    Serial.print("\n");

  int active_led_block_count=0;
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    if(g_active_led_block_list[i])
    {
      active_led_block_count++;
    }
  }

  int block_shortage = g_target_number_active_led_blocks - active_led_block_count;
  if(debugLevel>0)
  {
    Serial.print("active_led_block_count = ");
    Serial.print(active_led_block_count);
    Serial.print(" : target_led_block_count = ");
    Serial.print(g_target_number_active_led_blocks);
    Serial.print(" : block shortage = ");
    Serial.print(block_shortage);
    Serial.print("\n");
  }

  if(block_shortage>0)
  {
    // figure out what blocks to turn on by randomly selecting from the inactive list
    reset_inactive_led_block_list();

    if(g_num_inactive_led_blocks==0)
    {
        // no more blocks can be turned on - this seems like a problem
        Serial.print("Error: cannot turn on any more LED blocks");
        return;
    }

    // randomly select from this list by the shortage of active led blocks
    for(int i=0;i<block_shortage;i++)
    {
      // add 1 because function does max-1
      long new_index = random(0,g_num_inactive_led_blocks);
      if(debugLevel>0)
      {
        Serial.print("new_index = ");
        Serial.print(new_index);
        Serial.print("\n");
      }

      // get the block to make active
      int block_id = g_just_inactive_led_block_list[new_index];

      if(debugLevel>0)
      {
        Serial.print("choosing block ");
        Serial.print(block_id);
        Serial.print(" to activate\n");
      }

      // get the random color from the color list
      // actually this uses the color specified by the color pot
      // get the random brightness 
      // get the random mode
      // LEFT off here 10/12/2025
      //g_led_block[block_id].reinitialize(red, int green, int blue, float brightness_level, int mode);
      int random_color_index;
      float random_brightness_level;
      float random_brightness_change_rate;
      unsigned int random_mode;
      getRandomBlockSettings(random_color_index,random_brightness_level,random_brightness_change_rate,random_mode);

      // for this mode, use the color setting from the color pot
      uint8_t red = g_colorSample[random_color_index].red;
      uint8_t green = g_colorSample[random_color_index].green;
      uint8_t blue = g_colorSample[random_color_index].blue;
      reinitializeBlock(block_id,red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

            if(debugLevel>3)
      {
        Serial.print(" : r_bl = ");
        Serial.print(random_brightness_level);
        Serial.print(" : random_brightness_change_rate = ");
        Serial.print(random_brightness_change_rate);
        Serial.print(" : random_mode = ");
        Serial.print(random_mode);
        Serial.print("\n\n");
      }
    }

    if(debugLevel>0)
    {
      Serial.print("After resetting blocks\n");
      for(int i=0;i<NUM_LED_BLOCKS;i++)
      {
        Serial.print("led_block[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.print(g_led_block[i].m_active);
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    runOnceBlock(i);
  }

}


void getRandomBlockSettings(int &random_color_index,float &random_brightness_level, float &random_brightness_change_rate, unsigned int &random_mode)
{
  random_color_index = random(0,g_total_number_of_colors);


  int max_num_change_quantums = 100;
  // this will produce a slowing factor between [0.5,1.0] of the max time at a brightness level
  int change_quantum = random(25,max_num_change_quantums);

  random_brightness_change_rate = (float)change_quantum/(float)max_num_change_quantums;
  long random_mode_seed = random(0,101);
  if(random_mode_seed>50)
    random_mode=1;
  else
    random_mode=0;

  // force to always increasing
  random_mode=INCREASING;

  // number of brightness quantums
  int num_brightness_quantums = 100;
  if(random_mode==DECREASING)
  {
    int brightness_quantum = random(50,num_brightness_quantums);

    random_brightness_level = (float)brightness_quantum/(float)num_brightness_quantums;
  }
  else
  {
    int brightness_quantum = random(0,(long)(0.1*num_brightness_quantums));

    random_brightness_level = (float)brightness_quantum/(float)num_brightness_quantums;   
  }


}





void adjustColorBasedOnBrightness(uint8_t red,uint8_t green,uint8_t blue,float scaled_brightness,uint8_t &adj_red,uint8_t &adj_green,uint8_t &adj_blue)
{
   uint8_t new_red;
   uint8_t new_green;
   uint8_t new_blue;
   

   if(red>=green &&  red>=blue)
   {
	// red is max
	// compute ratios
	float green_to_red_ratio = (float)green/(float)red;
	float blue_to_red_ratio = (float)blue/(float)red;

        new_red = (uint8_t)(0.5 + scaled_brightness*red);
	new_green = (uint8_t)(0.5+(float)new_red*green_to_red_ratio);
	new_blue = (uint8_t)(0.5+(float)new_red*blue_to_red_ratio);	
   }
   else if( green>=red &&  green>=blue)
   {
	// green is max
	// compute ratios
	float red_to_green_ratio = (float)red/(float)green;
	float blue_to_green_ratio = (float)blue/(float)green;

        new_green = (uint8_t)(0.5+scaled_brightness*green);
	new_red = (uint8_t)(0.5+(float)new_green*red_to_green_ratio);
	new_blue = (uint8_t)(0.5+(float)new_green*blue_to_green_ratio);	
   }   
   else if( blue>=red &&  blue>=green)
   {
	// blue is max
	// compute ratios
	float red_to_blue_ratio = (float)red/(float) blue;
	float green_to_blue_ratio = (float)green/(float)blue;

        new_blue = (uint8_t)(0.5+scaled_brightness*blue);
	new_red = (uint8_t)(0.5+(float)new_blue*red_to_blue_ratio);
	new_green = (uint8_t)(0.5+(float)new_blue*green_to_blue_ratio);	
   }   

   adj_red = new_red;
   adj_green = new_green;
   adj_blue = new_blue;

#if 0   
   if(new_blue>1)
     {
   Serial.print("block ");
   Serial.print(m_blockID);
   Serial.print(": setting_id = ");
   Serial.print(m_settingID);
   Serial.print(" : mode = ");
   if(m_mode==INCREASING)
     Serial.print("INCREASING");
   else if(m_mode==DECREASING)
     Serial.print("DECREASING");   
   else
     {
       Serial.print("weird mode = ");
       Serial.print(m_mode);
     }
   Serial.print(" : in_red = ");
   Serial.print((int)red);
   Serial.print(" : in_green = ");
   Serial.print((int)green);
   Serial.print(" : in_blue = ");
   Serial.print((int)blue);
   Serial.print("out_red = ");
   Serial.print((int)adj_red);
   Serial.print(" : out_green = ");
   Serial.print((int)adj_green);
   Serial.print(" : out_blue = ");
   Serial.print((int)adj_blue);
   Serial.print("\n\n");
     }
#endif   
}


void reinitializeBlock(int block_id, uint8_t red,uint8_t green,uint8_t blue,float starting_brightness_level,float brightness_change_slowing_rate,int mode)
{
  // reinitialize directly
  g_led_block[block_id].m_origRed=red;
  g_led_block[block_id].m_origGreen=green;
  g_led_block[block_id].m_origBlue=blue;
  uint8_t adjustedRed;
  uint8_t adjustedGreen;
  uint8_t adjustedBlue;

  adjustColorBasedOnBrightness(red,green,blue,starting_brightness_level,adjustedRed,adjustedGreen,adjustedBlue);
  g_led_block[block_id].m_adjustedRed=adjustedRed;
  g_led_block[block_id].m_adjustedGreen=adjustedGreen;
  g_led_block[block_id].m_adjustedBlue=adjustedBlue;

  g_led_block[block_id].m_mode = mode;
  g_led_block[block_id].m_currentBrightnessLevel = starting_brightness_level;
  g_led_block[block_id].m_startingBrightnessLevel = starting_brightness_level;
  g_led_block[block_id].m_brightnessChangeSlowingRate = brightness_change_slowing_rate;
  g_led_block[block_id].m_startTimeMS = millis();
  g_led_block[block_id].m_timeAtCurrentLevelMS = 0;
  g_led_block[block_id].m_active=true;
  g_led_block[block_id].m_settingID++;


  if(0)
    {
      Serial.print("r_bl = ");
      Serial.print(g_led_block[block_id].m_currentBrightnessLevel);
      Serial.print(" : random_brightness_change_rate = ");
      Serial.print(g_led_block[block_id].m_brightnessChangeSlowingRate);
      Serial.print(" : random_mode = ");
      Serial.print(g_led_block[block_id].m_mode);
      Serial.print("\n\n");
    }
}

void runOnceBlock(int block_id)
{
  if(!g_led_block[block_id].m_active)
    {
      return;
    }
  // here is where the magic is occuring
  // LEFT off here 10/12/2025
  // compute some timing stuff ere
  long brightness_level_time_threshold = (long)(g_led_block[block_id].m_maxTimeAtBrightnessLevelMS*g_led_block[block_id].m_brightnessChangeSlowingRate);

  unsigned long current_time_ms = millis();
  long elapsed_time_ms = (long)current_time_ms - (long)g_led_block[block_id].m_startTimeMS;
#if 0
  Serial.print("LED block ");
  Serial.print(g_led_block[block_id].m_blockID);
  Serial.print(": start_time_ms = ");
  Serial.print(g_led_block[block_id].m_startTimeMS);
  Serial.print("\n\n");
#endif

#if 0  
  Serial.print(" : max_time_at_level_ms = ");
  Serial.print(g_led_block[block_id].m_maxTimeAtBrightnessLevelMS);
  Serial.print(" : slowing_rate = ");
  Serial.print(g_led_block[block_id].m_brightnessChangeSlowingRate);
  Serial.print(" : elapsed_time_ms = ");
  Serial.print(elapsed_time_ms);
  Serial.print(" : time_threshold_ms = ");
  Serial.print(brightness_level_time_threshold);
  Serial.print("\n\n");
#endif  
  if(elapsed_time_ms>=brightness_level_time_threshold)
    {
      // exceeded time at a brightness level so adjust brightness and then colors
      if(debugLevel>3)
      {
        Serial.print("brightness level = ");
        Serial.print(g_led_block[block_id].m_currentBrightnessLevel);
        Serial.print("\n");
      }
        
      if(g_led_block[block_id].m_mode==INCREASING)
      {
        if(debugLevel>3)
          {
            Serial.print("mode increasing\n");
          }
	  
        // increase brightness
        g_led_block[block_id].m_currentBrightnessLevel+=g_led_block[block_id].m_brightnessChangeAmount;
        if(debugLevel>3)
          {
            Serial.print("new brightness level = ");
            Serial.print(g_led_block[block_id].m_currentBrightnessLevel);
            Serial.print("\n");
          }
      
        if(g_led_block[block_id].m_currentBrightnessLevel>1.0)
          {
            // switch mode to decreasing since eventually every block fizzles out,
            // but here it goes up first then fizzles
            g_led_block[block_id].m_mode = DECREASING;
            // adjust for the above increase
            g_led_block[block_id].m_currentBrightnessLevel = g_led_block[block_id].m_currentBrightnessLevel - 2*g_led_block[block_id].m_brightnessChangeAmount;
            if(g_led_block[block_id].m_currentBrightnessLevel<0.0)
            {
              g_led_block[block_id].m_active=false;
              g_led_block[block_id].m_origRed=0;
              g_led_block[block_id].m_origGreen=0;
              g_led_block[block_id].m_origBlue=0;
              g_led_block[block_id].m_adjustedRed=0;
              g_led_block[block_id].m_adjustedGreen=0;
              g_led_block[block_id].m_adjustedBlue=0;	      
            }
          else
          {
            // recompute the adjusted colors base on the new brightness level
            adjustColorBasedOnBrightness(g_led_block[block_id].m_origRed,g_led_block[block_id].m_origGreen,g_led_block[block_id].m_origBlue,g_led_block[block_id].m_currentBrightnessLevel,
            g_led_block[block_id].m_adjustedRed,g_led_block[block_id].m_adjustedGreen,g_led_block[block_id].m_adjustedBlue);
            //color.setRGB(m_adjustedRed,m_adjustedGreen,m_adjustedBlue);
          }
	      }
  	  }
      else if(g_led_block[block_id].m_mode==DECREASING)
	    {
        if(debugLevel>3)
          {
            Serial.print("mode decreasing\n");
          }
	  
        g_led_block[block_id].m_currentBrightnessLevel-=g_led_block[block_id].m_brightnessChangeAmount;
        if(debugLevel>3)
          {
            Serial.print("new brightness level = ");
            Serial.print(g_led_block[block_id].m_currentBrightnessLevel);
            Serial.print("\n");
          }
        
        if(g_led_block[block_id].m_currentBrightnessLevel<0.0)
          {
            // this led block has completed its cycle so turn off leds and set active to false
            g_led_block[block_id].m_active=false;
            g_led_block[block_id].m_origRed=0;
            g_led_block[block_id].m_origGreen=0;
            g_led_block[block_id].m_origBlue=0;
            g_led_block[block_id].m_adjustedRed=0;
            g_led_block[block_id].m_adjustedGreen=0;
            g_led_block[block_id].m_adjustedBlue=0;	      
          }
        else
          {
            // recompute the adjusted colors base on the new brightness level
            adjustColorBasedOnBrightness(g_led_block[block_id].m_origRed,g_led_block[block_id].m_origGreen,g_led_block[block_id].m_origBlue,
            g_led_block[block_id].m_currentBrightnessLevel,g_led_block[block_id].m_adjustedRed,g_led_block[block_id].m_adjustedGreen,g_led_block[block_id].m_adjustedBlue);
          }
	      }
      else
      {
        g_led_block[block_id].m_origRed=0;
        g_led_block[block_id].m_origGreen=0;
        g_led_block[block_id].m_origBlue=0;
        g_led_block[block_id].m_adjustedRed=0;
        g_led_block[block_id].m_adjustedGreen=0;
        g_led_block[block_id].m_adjustedBlue=0;
        g_led_block[block_id].m_active=false;
      }
    }
  else
    {
      adjustColorBasedOnBrightness(g_led_block[block_id].m_origRed,g_led_block[block_id].m_origGreen,g_led_block[block_id].m_origBlue,g_led_block[block_id].m_currentBrightnessLevel,
      g_led_block[block_id].m_adjustedRed,g_led_block[block_id].m_adjustedGreen,g_led_block[block_id].m_adjustedBlue);

#if 0      
      Serial.print("keep at same color value");
      Serial.print("   red = ");
      Serial.print(color.red);
      Serial.print(" : green = ");
      Serial.print(color.green);
      Serial.print(" : blue = ");
      Serial.print(color.blue);            
      Serial.print("\n");
#endif      
    }

  CRGB color;
  color.red = g_led_block[block_id].m_adjustedRed;
  color.green = g_led_block[block_id].m_adjustedGreen;
  color.blue = g_led_block[block_id].m_adjustedBlue;      
  color.r = g_led_block[block_id].m_adjustedRed;
  color.g = g_led_block[block_id].m_adjustedGreen;
  color.b = g_led_block[block_id].m_adjustedBlue;      

#if 0
  uint8_t color_red = color.r;
  uint8_t color_green = color.g;
  uint8_t color_blue = color.b;  
  Serial.print((int)color_red);
  Serial.print((int)color_green);
  Serial.print((int)color_blue);  
  Serial.print("   orig_red = ");
  Serial.print((int)g_led_block[block_id].m_origRed);
  Serial.print(" : orig_green = ");
  Serial.print((int)g_led_block[block_id].m_origGreen);
  Serial.print(" : orig_blue = ");
  Serial.print((int)g_led_block[block_id].m_origBlue);

  Serial.print("   : adj_red = ");
  Serial.print((int)g_led_block[block_id].m_adjustedRed);
  Serial.print(" : adj_green = ");
  Serial.print((int)g_led_block[block_id].m_adjustedGreen);
  Serial.print(" : adj_blue = ");
  Serial.print((int)g_led_block[block_id].m_adjustedBlue);
  Serial.print("\n");
#endif
#if 0
      Serial.print((int)color.red);

      Serial.print("   : color_green = ");
      Serial.print((int)color.green);
      Serial.print("   : color_blue = ");
      Serial.print((int)color.blue);

      Serial.print("\n");
#endif


#if 0      
  for(int i=m_startLED;i<=m_endLED;i++)    
  {
    if(g_leds[i].blue>1)
    {
      Serial.print("     led[");
      Serial.print(i);
      Serial.print("]: red = ");
      Serial.print(leds[i].red);
      Serial.print(" : green = ");
      Serial.print(leds[i].green);
      Serial.print(" : blue = ");
      Serial.print(leds[i].blue);
      Serial.print("\n");
    }
  }
  Serial.print("\n\n\n");
#endif


  for(uint8_t i=g_led_block[block_id].m_startLED;i<=g_led_block[block_id].m_endLED;i++)
    {
      g_leds[i] = color;
    }
}
