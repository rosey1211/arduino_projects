#include "ledBlock.h"

ledBlock::ledBlock()
{
  
}

ledBlock::~ledBlock()
{

}
#if 0
unsigned ledBlock::getStartLED()
{
  return m_startLED;
}

unsigned ledBlock::getEndLED()
{
  return m_endLED;
}

bool ledBlock::getActive()
{
  return m_active;
}

void ledBlock::setActive(bool val)
{
  m_active = val;
}

void ledBlock::runOnce(CRGB *leds)
{
  if(!m_active)
    {
      return;
    }
  // here is where the magic is occuring
  // LEFT off here 10/12/2025
  // compute some timing stuff ere
  m_maxTimeAtBrightnessLevelMS;
  long brightness_level_time_threshold = (long)(m_maxTimeAtBrightnessLevelMS*m_brightnessChangeSlowingRate);

  unsigned long current_time_ms = millis();
  long elapsed_time_ms = (long)current_time_ms - (long)m_startTimeMS;
#if 0
  Serial.print("LED block ");
  Serial.print(m_blockID);
  Serial.print(": start_time_ms = ");
  Serial.print(m_startTimeMS);
  Serial.print("\n\n");
#endif

#if 0  
  Serial.print(" : max_time_at_level_ms = ");
  Serial.print(m_maxTimeAtBrightnessLevelMS);
  Serial.print(" : slowing_rate = ");
  Serial.print(m_brightnessChangeSlowingRate);
  Serial.print(" : elapsed_time_ms = ");
  Serial.print(elapsed_time_ms);
  Serial.print(" : time_threshold_ms = ");
  Serial.print(brightness_level_time_threshold);
  Serial.print("\n\n");
#endif  
  if(elapsed_time_ms>=brightness_level_time_threshold)
    {
      // exceeded time at a brightness level so adjust brightness and then colors
      if(m_debugLevel>3)
	{
	  Serial.print("brightness level = ");
	  Serial.print(m_currentBrightnessLevel);
	  Serial.print("\n");
	}
      
      if(m_mode==INCREASING)
	{
	  if(m_debugLevel>3)
	    {
	      Serial.print("mode increasing\n");
	    }
	  
	  // increase brightness
	  m_currentBrightnessLevel+=m_brightnessChangeAmount;
	  if(m_debugLevel>3)
	    {
		  Serial.print("new brightness level = ");
		  Serial.print(m_currentBrightnessLevel);
		  Serial.print("\n");
	    }
	  
	  if(m_currentBrightnessLevel>1.0)
	    {
	      // switch mode to decreasing since eventually every block fizzles out,
	      // but here it goes up first then fizzles
	      m_mode = DECREASING;
	      // adjust for the above increase
	      m_currentBrightnessLevel = m_currentBrightnessLevel - 2*m_brightnessChangeAmount;
	      if(m_currentBrightnessLevel<0.0)
		{
		  m_active=false;
		  m_origRed=0;
		  m_origGreen=0;
		  m_origBlue=0;
		  m_adjustedRed=0;
		  m_adjustedGreen=0;
		  m_adjustedBlue=0;	      
		}
	      else
		{
		  // recompute the adjusted colors base on the new brightness level
		  adjustColorBasedOnBrightness(m_origRed,m_origGreen,m_origBlue,m_currentBrightnessLevel,m_adjustedRed,m_adjustedGreen,m_adjustedBlue);
		  //color.setRGB(m_adjustedRed,m_adjustedGreen,m_adjustedBlue);
		}
	    }
	}
      else if(m_mode==DECREASING)
	{
	  if(m_debugLevel>3)
	    {
	      Serial.print("mode decreasing\n");
	    }
	  
	  m_currentBrightnessLevel-=m_brightnessChangeAmount;
	  if(m_debugLevel>3)
	    {
	      Serial.print("new brightness level = ");
	      Serial.print(m_currentBrightnessLevel);
	      Serial.print("\n");
	    }
	  
	  if(m_currentBrightnessLevel<0.0)
	    {
	      // this led block has completed its cycle so turn off leds and set active to false
	      m_active=false;
	      m_origRed=0;
	      m_origGreen=0;
	      m_origBlue=0;
	      m_adjustedRed=0;
	      m_adjustedGreen=0;
	      m_adjustedBlue=0;	      
	    }
	  else
	    {
	      // recompute the adjusted colors base on the new brightness level
	      adjustColorBasedOnBrightness(m_origRed,m_origGreen,m_origBlue,m_currentBrightnessLevel,m_adjustedRed,m_adjustedGreen,m_adjustedBlue);
	    }
	}
      else
	{
	  m_origRed=0;
	  m_origGreen=0;
	  m_origBlue=0;
	  m_adjustedRed=0;
	  m_adjustedGreen=0;
	  m_adjustedBlue=0;
	  m_active=false;
	}
    }
  else
    {
      adjustColorBasedOnBrightness(m_origRed,m_origGreen,m_origBlue,m_currentBrightnessLevel,m_adjustedRed,m_adjustedGreen,m_adjustedBlue);

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
  color.red = m_adjustedRed;
  color.green = m_adjustedGreen;
  color.blue = m_adjustedBlue;      
  color.r = m_adjustedRed;
  color.g = m_adjustedGreen;
  color.b = m_adjustedBlue;      

#if 0
  uint8_t color_red = color.r;
  uint8_t color_green = color.g;
  uint8_t color_blue = color.b;  
  Serial.print((int)color_red);
  Serial.print((int)color_green);
  Serial.print((int)color_blue);  
      Serial.print("   orig_red = ");
      Serial.print((int)m_origRed);
      Serial.print(" : orig_green = ");
      Serial.print((int)m_origGreen);
      Serial.print(" : orig_blue = ");
      Serial.print((int)m_origBlue);

      Serial.print("   : adj_red = ");
      Serial.print((int)m_adjustedRed);
      Serial.print(" : adj_green = ");
      Serial.print((int)m_adjustedGreen);
      Serial.print(" : adj_blue = ");
      Serial.print((int)m_adjustedBlue);
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
    if(leds[i].blue>1)
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


  for(uint8_t i=m_startLED;i<=m_endLED;i++)
    {
      leds[i] = color;
    }
}

void ledBlock::setup(unsigned int startLED,unsigned int endLED,unsigned int block_id,int max_time_at_brightness_level,float brightness_change_amount) 
{
  m_startLED = startLED;
  m_endLED = endLED;
  m_blockID = block_id;
  m_maxTimeAtBrightnessLevelMS = max_time_at_brightness_level;
  m_brightnessChangeAmount = brightness_change_amount;
  m_active = false;
  m_debugLevel=0;
  m_settingID=0;
  m_mode=500;
}

void ledBlock::reset()
{
  m_active=false;
}

void ledBlock::reinitialize(uint8_t red,uint8_t green,uint8_t blue,float starting_brightness_level,float brightness_change_slowing_rate,int mode)
{
  m_origRed=red;
  m_origGreen=green;
  m_origBlue=blue;
  adjustColorBasedOnBrightness(m_origRed,m_origGreen,m_origBlue,starting_brightness_level,m_adjustedRed,m_adjustedGreen,m_adjustedBlue);
  m_mode = mode;
  m_currentBrightnessLevel = m_startingBrightnessLevel = starting_brightness_level;
  m_brightnessChangeSlowingRate = brightness_change_slowing_rate;
  m_startTimeMS = millis();
  m_timeAtCurrentLevelMS = 0;
  m_active=true;
  m_settingID++;

  if(0)
    {
      Serial.print("r_bl = ");
      Serial.print(m_currentBrightnessLevel);
      Serial.print(" : random_brightness_change_rate = ");
      Serial.print(m_brightnessChangeSlowingRate);
      Serial.print(" : random_mode = ");
      Serial.print(m_mode);
      Serial.print("\n\n");
    }
}


void ledBlock::adjustColorBasedOnBrightness(uint8_t red,uint8_t green,uint8_t blue,float scaled_brightness,uint8_t &adj_red,uint8_t &adj_green,uint8_t &adj_blue)
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
#endif // big if 0
