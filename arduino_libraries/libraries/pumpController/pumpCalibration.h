#ifndef PUMPCALIBRATION_H
#define PUMPCALIBRATION_H


#include <Arduino.h>

#define CALIBRATION_SIZE 14

class pumpCalibration
{
 public:
  pumpCalibration();
  ~pumpCalibration();
  bool fillInInvalidSpoutHeightElements();
  void printCalibration();
  bool getPWMFromRawSpoutHeight(float spout_height, int &pwm_level);
  bool getPWMFromScaledSpoutHeight(float scaled_spout_height, int &pwm_level);  
  
private:
  int m_debugLevel = 1;
  float m_pwmLevel[CALIBRATION_SIZE];
  float m_spoutHeight[CALIBRATION_SIZE];
  float m_scaledSpoutHeight[CALIBRATION_SIZE];  
  float m_baseHeight = 7.5;
  float m_minimumSpoutHeight;
  float m_maximumSpoutHeight;  
};

#endif
