


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
      num_block_leds=4;
    }

    end_led=start_led+num_block_leds-1;
    g_led_block[i].setup(start_led,end_led,i,g_maxTimeAtBrightnessLevelMS,g_brightnessChangeAmount);

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
  if(g_just_inactive_led_block_list!=NULL)
  {
    free(g_just_inactive_led_block_list);
    g_just_inactive_led_block_list=NULL;
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
    g_just_inactive_led_block_list = (int *)NULL;
    g_num_inactive_led_blocks = 0;
    return;
  }

  g_just_inactive_led_block_list = (int *)malloc(sizeof(int)*inactive_count);
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
      Serial.print(g_led_block[i].getActive());
      Serial.print(" : ");
    }
    if(g_led_block[i].getActive())
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

      g_led_block[block_id].reinitialize(red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

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
        Serial.print(g_led_block[i].getActive());
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_led_block[i].runOnce(g_leds);
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
      Serial.print(g_led_block[i].getActive());
      Serial.print(" : ");
    }
    if(g_led_block[i].getActive())
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

      g_led_block[block_id].reinitialize(red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

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
        Serial.print(g_led_block[i].getActive());
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_led_block[i].runOnce(g_leds);
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
      Serial.print(g_led_block[i].getActive());
      Serial.print(" : ");
    }
    if(g_led_block[i].getActive())
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

      g_led_block[block_id].reinitialize(red,green,blue,random_brightness_level,random_brightness_change_rate,random_mode);

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
        Serial.print(g_led_block[i].getActive());
        Serial.print(" : ");
      }
      Serial.print("\n\n\n");
    }
  }

  // if here, we should have the proper number of active LED blocks
  for(int i=0;i<NUM_LED_BLOCKS;i++)
  {
    g_led_block[i].runOnce(g_leds);
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
