#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H


#include <Arduino.h>
#include "pumpCalibration.h"

class pumpController
{
 public:
  pumpController();
  ~pumpController();
  void setupPump(char *name,int inlo_pin,int inhi_pin,int en_pin,float height_scale_factor);
  void setPumpLevel(float val);
  void hardStop();
  float getPumpLevel();
  void setInLoPin(int val);
  void setInHiPin(int val);
  bool setEnPin(int val);
  bool setLoVal(int val);
  bool setHiVal(int val);
  bool setEnVal(int val);
  void setHeightScaleFactor(float val);

  void setName(char *s);
  void setDebugLevel(int val);
  void printMessage(char *s);
  void printPumpInfo();

  float m_currentLevel;
  uint8_t m_minimum_pwm_output=30;
  float m_heightScaleFactor=1.0;
  bool m_initialized = false;
  bool m_use = true;
  
private:
  uint8_t m_inLoPin;
  uint8_t m_inHiPin;
  uint8_t m_enPin;
  uint8_t m_id;
  char m_name[20];
  pumpCalibration m_calibration;
  // 0 is no debug, greater is more debug
  uint8_t m_debugLevel = 0;
};

#endif
