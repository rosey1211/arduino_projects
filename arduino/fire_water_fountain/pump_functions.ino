
#if 0 // done below
int current_pump=0;
unsigned long max_time_at_level_ms = 2000;
unsigned long current_time_at_level_ms=0;
unsigned long start_time_for_pump_ms=0;
#endif

bool first_time_pump=true;

bool m_debug_pump=true;
bool lastPumpProgramButton = LOW;
int current_pump_program_id = 0;
float left_pump_scale_factor = 0.95;
float center_pump_scale_factor = 0.95;
float right_pump_scale_factor = 1.0;

bool pump_program_changed = false;

void readPumpProgramButton()
{
  bool currentButton = digitalRead(PUMP_PROGRAM_TOGGLE);
  if(currentButton==HIGH && lastPumpProgramButton==LOW)
  {
    current_pump_program_id = (current_pump_program_id+1)%MAX_PUMP_PROGRAMS;
    Serial.print("pump program = ");
    Serial.print(current_pump_program_id);
    Serial.print("\n");
    lastPumpProgramButton = HIGH;
    pump_program_changed = true;
  }
  if(currentButton==LOW && lastPumpProgramButton==HIGH)
  {
    // reset
    lastPumpProgramButton=LOW;
  }
}


void setupPumps()
{
//  Serial.print("setup pumps\n");
  bool use_pump_left = true;
  bool use_pump_center = true;
  bool use_pump_right = true;

  // this will setup pumps and do the proper pin assignments on the arduino
  if(!use_pumps)
  {
    return;
  }

  for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {
    pump_controller[p].setDebugLevel(debugLevel);
#if 0
    Serial.print("setting up pump ");
    Serial.print(p);
    Serial.print("\n");
#endif    
    char name[100];
    if(p==LEFT)
    {
      if(use_pump_left)
      {
        // left
        sprintf(name,"left_pump");
        pump_controller[p].setupPump(name,LEFT_PUMP_INLO_PIN,LEFT_PUMP_INHI_PIN,LEFT_PUMP_EN_PIN,left_pump_scale_factor);
        pump_controller[p].m_use = true;
      } 
      else
      {
        pump_controller[p].m_use = false;
      }
#if 0      
      Serial.print("left pump used = ");
      Serial.print(pump_controller[p].m_use);
      Serial.print("\n");
#endif
    }
    else if(p==CENTER)
    {
      if(use_pump_center)
      {
        // center
        sprintf(name,"center_pump");
        pump_controller[p].setupPump(name,CENTER_PUMP_INLO_PIN,CENTER_PUMP_INHI_PIN,CENTER_PUMP_EN_PIN,center_pump_scale_factor); 
        pump_controller[p].m_use = true;
      }
      else
      {
        pump_controller[p].m_use = false;
      }
#if 0
      Serial.print("center pump used = ");
      Serial.print(pump_controller[p].m_use);
      Serial.print("\n");
#endif      
    }
    else if(p==RIGHT)
    {
#if 0      
      Serial.print("use_pump_right = ");
      Serial.print(use_pump_right);
      Serial.print("\n");
#endif      
      if(use_pump_right)
      {
        // right
        sprintf(name,"right_pump");
        pump_controller[p].setupPump(name,RIGHT_PUMP_INLO_PIN,RIGHT_PUMP_INHI_PIN,RIGHT_PUMP_EN_PIN,right_pump_scale_factor);
        pump_controller[p].m_use = true; 
      }
      else
      {
        pump_controller[p].m_use = false;
      }
#if 0      
      Serial.print("right pump used = ");
      Serial.print(pump_controller[p].m_use);
      Serial.print("\n");
#endif      
    }
  }
}

void runPumpProgram()
{
  if(current_pump_program_id==PUMPS_RANDOM_SYNCHED_UP_DOWN)
  {
    pumpsRandomSynchedUpDown();
  }
  else if(current_pump_program_id==PUMPS_CHANGING_ANGLE)
  {
    pumpsAlternatingAngle();
  }
  else if(current_pump_program_id==PUMPS_RANDOM)
  {
    pumpsRandom();
  }
  else if(current_pump_program_id==ALTERNATING_HEIGHTS)
  {
    pumpsAlternatingHeights();
  }
  else if(current_pump_program_id==MAX_HEIGHTS)
  {
    pumpsMaxHeights();
  }
  else if(current_pump_program_id==MAX_RAW_HEIGHTS)
  {
    pumpsMaxRawHeights();
  }  
}
void pumpsMaxHeights()
{
 for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {  
    if(!pump_controller[p].m_use)
    {
      continue;
    }

    pump_controller[p].setPumpLevel(1.0); 
  }

  if(pump_program_changed)
  {
    pump_program_changed = false;
  }
}

void pumpsMaxRawHeights()
{
 for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {  
    if(!pump_controller[p].m_use)
    {
      continue;
    }

    pump_controller[p].setEnVal(255); 
  }
  if(pump_program_changed)
  {
    pump_program_changed = false;
  }
}

void pumpsRandomSynchedUpDown()
{
  // pick a random number between [0,1]
  // set all pumps to the same value
  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 3000.0;
  static bool first_time=true;
  static long level_start_time_ms = millis();
  
  if(pump_program_changed)
  {
    first_time=true;
    pump_program_changed = false;
  }
  
  if(first_time)
  {
    level_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - level_start_time_ms;
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
  level_start_time_ms = millis();


  float random_level = (float)random(0,255)/255.0;

  for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {  
    if(!pump_controller[p].m_use)
    {
      continue;
    }

    pump_controller[p].setPumpLevel(random_level); 
  }
}

void pumpsAlternatingAngle()
{
  static bool positive_slope;

  // pick a random number between [0,1]
  // set all pumps to the same value
  float slow_factor = 0.5; // between [0,1]
  float max_delay_ms = 100.0;
  static bool first_time=true;
  static long level_start_time_ms = millis();
  float minimum_pump_level = 0.1;
  if(pump_program_changed)
  {
    first_time=true;
    pump_program_changed = false;
  }


  if(first_time)
  {
    level_start_time_ms = millis();
    pump_controller[1].setPumpLevel(0.6);
    pump_controller[0].setPumpLevel(minimum_pump_level);
    pump_controller[2].setPumpLevel(1.0);
    positive_slope=true;
#if 0
    if(positive_slope)
    {
      Serial.print("positive_slope");
    }
    else
    {
      Serial.print("negative_slope");
    }
#endif
#if 0    
    Serial.print("[0] ");
    Serial.print(pump_controller[0].getPumpLevel());
    Serial.print(" : [1] ");
    Serial.print(pump_controller[1].getPumpLevel());
    Serial.print(" : [2] ");
    Serial.print(pump_controller[2].getPumpLevel());  
    Serial.print("\n");
#endif
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - level_start_time_ms;
  float time_threshold_ms = max_delay_ms*slow_factor;
  //Serial.print(elapsed_time_ms);
  //Serial.print("\n");
  //if(!first_time)
  if(elapsed_time_ms<time_threshold_ms)
  {
    first_time=false;
    return;
  }

  // if here, we are ready for the next color
  level_start_time_ms = millis();
  
  float pump_change=0.01;
  if(positive_slope)
  {
    float rising_pump_level = pump_controller[0].getPumpLevel();
    float new_rising_pump_level = rising_pump_level+pump_change;
    float falling_pump_level = pump_controller[2].getPumpLevel();
    float new_falling_pump_level = falling_pump_level-pump_change;
#if 0
    Serial.print("In positive slope: new_rising = ");
    Serial.print(new_rising_pump_level);
    Serial.print(" : new_falling = ");
    Serial.print(new_falling_pump_level);
    Serial.print("\n");
#endif    
    if(new_rising_pump_level>1.001)//  || new_falling_pump_level<=-pump_change)
    {
        positive_slope = !positive_slope;
#if 0
        Serial.print("flipping from positive to negative");
        Serial.print("\n");
#endif        
        return;
    }

    // final check
    if(new_rising_pump_level>1.0) new_rising_pump_level=1.0;
    if(new_falling_pump_level<0.0) new_falling_pump_level=0.0;
    pump_controller[0].setPumpLevel(new_rising_pump_level);
    pump_controller[2].setPumpLevel(new_falling_pump_level);    
  }
  else if(!positive_slope)
  {
    float rising_pump_level = pump_controller[2].getPumpLevel();
    float new_rising_pump_level = rising_pump_level+pump_change;
    float falling_pump_level = pump_controller[0].getPumpLevel();
    float new_falling_pump_level = falling_pump_level-pump_change;
#if 0
    Serial.print("In negative slope: new_rising = ");
    Serial.print(new_rising_pump_level);
    Serial.print(" : new_falling = ");
    Serial.print(new_falling_pump_level);
    Serial.print("\n");
#endif
    if(new_rising_pump_level>1.001)//  || new_falling_pump_level<=-pump_change)
    {
#if 0
        Serial.print("flipping from negative to positive");
        Serial.print("\n");
#endif 
        positive_slope = !positive_slope;
        return;
    }

    // final check
    if(new_rising_pump_level>1.0) new_rising_pump_level=1.0;
    if(new_falling_pump_level<0.0) new_falling_pump_level=0.0;
 
    pump_controller[2].setPumpLevel(new_rising_pump_level);
    pump_controller[0].setPumpLevel(new_falling_pump_level);    
  }  

#if 0
  Serial.print("[0] ");
  Serial.print(pump_controller[0].getPumpLevel());
  Serial.print(" : [1] ");
  Serial.print(pump_controller[1].getPumpLevel());
  Serial.print(" : [2] ");
  Serial.print(pump_controller[2].getPumpLevel());  
  Serial.print("\n");
#endif
  first_time=false;
}

void pumpsRandom()
{
  // pick a random number between [0,1]
  // set all pumps to the same value
  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 3000.0;
  static bool first_time=true;
  static long level_start_time_ms = 0;

  if(pump_program_changed)
  {
    first_time=true;
    pump_program_changed = false;
  }

  if(first_time)
  {
    level_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - level_start_time_ms;
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
  level_start_time_ms = millis();



  for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {    
    float random_level = (float)random(0,255)/255.0;
    if(!pump_controller[p].m_use)
    {
      continue;
    }
    
    pump_controller[p].setPumpLevel(random_level); 
  }
}

void pumpsAlternatingHeights()
{
  static int phase=0;
  // pick a random number between [0,1]
  // set all pumps to the same value
  float slow_factor = 1.0; // between [0,1]
  float max_delay_ms = 3000.0;
  static bool first_time=true;
  static long level_start_time_ms = 0;

  if(pump_program_changed)
  {
    first_time=true;
    pump_program_changed = false;
  }


  if(first_time)
  {
    level_start_time_ms = millis();
  }

  unsigned long current_time_ms = millis();
  unsigned int elapsed_time_ms = current_time_ms - level_start_time_ms;
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
  level_start_time_ms = millis();

  
  if(phase==0)
  {
    phase=1;
  }
  else if(phase==1)
  {
    phase=1;
  }


  if(phase==0)
  {
      // outers are high and middle is middle
      if(pump_controller[0].m_use)
      {
        pump_controller[0].setPumpLevel(1.0);
      }
      if(pump_controller[NUMBER_OF_PUMPS-1].m_use)
      {
        pump_controller[NUMBER_OF_PUMPS-1].setPumpLevel(1.0);
      }
      for(int p=1;p<NUMBER_OF_PUMPS-1;p++)
      {
        pump_controller[p].setPumpLevel(0.2);
      }
  }
  else if(phase==1)
  {
      // outers are high and middle is middle
      if(pump_controller[0].m_use)
      {
        pump_controller[0].setPumpLevel(0.2);
      }
      if(pump_controller[NUMBER_OF_PUMPS-1].m_use)
      {
        pump_controller[NUMBER_OF_PUMPS-1].setPumpLevel(0.2);
      }
      for(int p=1;p<NUMBER_OF_PUMPS-1;p++)
      {
        pump_controller[p].setPumpLevel(1.0);
      }
  }

  for(int p=0;p<NUMBER_OF_PUMPS;p++)
  {    
    float random_level = (float)random(0,255)/255.0;
    if(!pump_controller[p].m_use)
    {
      continue;
    }
    
    pump_controller[p].setPumpLevel(random_level); 
  }
}



float current_pump_level = 0;
unsigned long current_time_at_level_ms=0;
unsigned long start_time_for_pump_ms=0;  
uint8_t current_pump=0;
float pump_level_increment = 0.05;
unsigned long max_time_at_level_ms = 2000;

bool pump_test()
{
  if(current_pump==NUMBER_OF_PUMPS)
  {
    // test completed
    if(m_debug_pump)
    {
      Serial.print("Completed tests on all pumps\n");
    }
    for(int p=0;p<NUMBER_OF_PUMPS;p++)
    {
      if(pump_controller[p].m_use)
      {
        //pump_controller[p].hardStop();
        pump_controller[p].setPumpLevel(0.0);
      } 
    }
    return false;
  }

  if(first_time_pump)
  {
    start_time_for_pump_ms = millis();
    current_pump_level = 0.0;
    first_time_pump=false;
    for(int p=0;p<NUMBER_OF_PUMPS;p++)
    {
      if(pump_controller[p].m_use)
      {
        pump_controller[p].hardStop();
        //pump_controller[p].setPumpLevel(0.0);
      } 
    }
  }
  
  if(!pump_controller[current_pump].m_use)
  {
    if(m_debug_pump)
    {
#if 0
      Serial.print("Pump ");
      Serial.print(current_pump);
      Serial.print(" not used\n");
#endif      
    }
    
    current_pump++;
    if(current_pump==NUMBER_OF_PUMPS)
    {
      if(m_debug_pump)
      {
        Serial.print("Completed pump testing\n");
      }
      for(int p=0;p<NUMBER_OF_PUMPS;p++)
      {
        if(pump_controller[p].m_use)
        {
          pump_controller[p].setPumpLevel(0.0); 
        }
      }

      return false;
    }
  }

  unsigned long int current_time_ms = millis();
  current_time_at_level_ms = current_time_ms - start_time_for_pump_ms;
#if 0
  Serial.print("current_time_at_level_ms = ");
  Serial.print(current_time_at_level_ms);
  Serial.print(" : current_time_ms = ");
  Serial.print(current_time_ms);
  Serial.print(" : start_time_for_pump_ms = ");
  Serial.print(start_time_for_pump_ms);
  Serial.print("\n");
#endif  
  if(current_time_at_level_ms>=max_time_at_level_ms)
  {
    if(m_debug_pump)
    {
#if 0
      Serial.print("Pump ");
      Serial.print(current_pump);
      Serial.print(" level ");
      Serial.print(current_pump_level);
      Serial.print(" completed\n");
#endif
    }
    
    if(pump_controller[current_pump].m_use)
    {
      pump_controller[current_pump].setPumpLevel(0.0); 
    }

    current_time_at_level_ms = 0;
    start_time_for_pump_ms = millis();
    current_pump_level+=pump_level_increment;
  
    if(current_pump_level>=1.0)
    {
      // increment pump
      // send a 0 pump level to the current pump
      if(pump_controller[current_pump].m_use)
      {
        pump_controller[current_pump].hardStop(); 

//        pump_controller[current_pump].setPumpLevel(0.0); 
      }

      current_pump++;
      current_pump_level = 0.0;
      if(current_pump>=NUMBER_OF_PUMPS)
      {
         if(m_debug_pump)
          {
            Serial.print("Completed pump testing");
          }
          return false;
      }
    }
  }
  else
  {
    if(m_debug_pump)
    {
#if 0      
      Serial.print("Pump ");
      Serial.print(current_pump);
      Serial.print(" level ");
      Serial.print(current_pump_level);
      Serial.print(" : current_time_in_level = ");
      Serial.print(current_time_at_level_ms);
      Serial.print(" : max_time_at_level_ms = ");
      Serial.print(max_time_at_level_ms);
      Serial.print("\n");
#endif
    }
    if(pump_controller[current_pump].m_use)
    {
      pump_controller[current_pump].setPumpLevel(current_pump_level); 
    }
  }
  
  // still in test
  return true;
}