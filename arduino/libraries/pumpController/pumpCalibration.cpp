#include "pumpCalibration.h"

pumpCalibration::pumpCalibration()
{
  // setup the conversion tables between pwm level and spout height
  m_pwmLevel[0] = 10.0;
  m_pwmLevel[1] = 20.0;  
  m_pwmLevel[2] = 30.0;  
  m_pwmLevel[3] = 40.0;  
  m_pwmLevel[4] = 50.0;  
  m_pwmLevel[5] = 60.0;  
  m_pwmLevel[6] = 70.0;  
  m_pwmLevel[7] = 80.0;  
  m_pwmLevel[8] = 90.0;  
  m_pwmLevel[9] = 100.0;  
  m_pwmLevel[10] = 110.0;  
  m_pwmLevel[11] = 120.0;  
  m_pwmLevel[12] = 130.0;  
  m_pwmLevel[13] = 140.0;



  // fill in the spout elements with data
  m_spoutHeight[0] = 9.0 - m_baseHeight;
  m_spoutHeight[1] = 14.0 - m_baseHeight;
  m_spoutHeight[2] = 20.0 - m_baseHeight;
  m_spoutHeight[3] = 25.0 - m_baseHeight;
  m_spoutHeight[4] = 28.0 - m_baseHeight;
  m_spoutHeight[5] = 30.0 - m_baseHeight;
  m_spoutHeight[6] = 31.5 - m_baseHeight;
  m_spoutHeight[7] = 32.5 - m_baseHeight;
  m_spoutHeight[8] = 33.0 - m_baseHeight;
  m_spoutHeight[9] = 33.5 - m_baseHeight;
  m_spoutHeight[10] = 34.5 - m_baseHeight;
  m_spoutHeight[11] = 35.0 - m_baseHeight;
  m_spoutHeight[12] = 35.5 - m_baseHeight;
  m_spoutHeight[13] = 36.0 - m_baseHeight;

  m_minimumSpoutHeight = m_spoutHeight[0];
  m_maximumSpoutHeight = m_spoutHeight[CALIBRATION_SIZE-1];

  for(int i=0;i<CALIBRATION_SIZE;i++)
    {
      m_scaledSpoutHeight[i] = (m_spoutHeight[i] - m_minimumSpoutHeight)/(m_maximumSpoutHeight-m_minimumSpoutHeight);
    }
}



pumpCalibration::~pumpCalibration()
{

}



bool pumpCalibration::getPWMFromRawSpoutHeight(float spout_height, int &pwm_level)
{
  float sc_spout_height=(spout_height-m_minimumSpoutHeight)/(m_maximumSpoutHeight - m_minimumSpoutHeight);
  if(sc_spout_height>1.0)
    {
      sc_spout_height=1.0;
    }
  if(sc_spout_height<=0.0)
    {
      sc_spout_height=0.0;
    }
  return(getPWMFromScaledSpoutHeight(sc_spout_height,pwm_level));
}

bool pumpCalibration::getPWMFromScaledSpoutHeight(float scaled_spout_height, int &pwm_level)
{
  if(scaled_spout_height>=1.0)
    {
      pwm_level = m_pwmLevel[CALIBRATION_SIZE-1];
      return true;
    }
  if(scaled_spout_height<=0.0)
    {
      pwm_level = m_pwmLevel[0];      
      return true;      
    }



  // nominal case
  int first_element_index=-1;
  int last_element_index=-1;  
  int first_element_value=-1000000.0;
  int last_element_value=-1000000.0;  
  for(int i=0;i<CALIBRATION_SIZE-1;i++)
    {
      if(scaled_spout_height>=m_scaledSpoutHeight[i] &&
	 scaled_spout_height<m_scaledSpoutHeight[i+1])
	{
	  float min_pwm = m_pwmLevel[i];
	  float max_pwm = m_pwmLevel[i+1];
	  float min_sc_height = m_scaledSpoutHeight[i];
	  float max_sc_height = m_scaledSpoutHeight[i+1];	  
	  pwm_level = min_pwm+(max_pwm-min_pwm)*(scaled_spout_height-min_sc_height)/(max_sc_height-min_sc_height);
	}
    }
}

void pumpCalibration::printCalibration()
{
  char line[1000];
  for(int i=0;i<CALIBRATION_SIZE;i++)
    {
      int pwm_level;
      getPWMFromScaledSpoutHeight(m_scaledSpoutHeight[i],pwm_level);
      Serial.print("       spoutHeight[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(m_spoutHeight[i]);
      Serial.print(" : scaledSpoutHeight[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(m_scaledSpoutHeight[i]);
      Serial.print(" : pwm_level = ");
      Serial.print(pwm_level);
      Serial.print("\n");
    }
}
