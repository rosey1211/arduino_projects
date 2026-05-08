#include "pumpController.h"

pumpController::pumpController()
{

}



pumpController::~pumpController()
{

}

void pumpController::setupPump(char *name,int inlo_pin,int inhi_pin,int en_pin,float height_scale_factor)
{
  setName(name);
  setInLoPin(inlo_pin);
  setInHiPin(inhi_pin);
  setEnPin(en_pin);
  setHeightScaleFactor(height_scale_factor);


  pinMode(en_pin, OUTPUT);
  pinMode(inlo_pin, OUTPUT);
  pinMode(inhi_pin, OUTPUT);

  // Set initial rotation direction
  digitalWrite(inlo_pin, HIGH);
  digitalWrite(inhi_pin, LOW);

  // initialize the water column at half height
  analogWrite(en_pin, m_minimum_pwm_output); // Send PWM signal to L298N Enable pin

  m_initialized=true;
}

float pumpController::getPumpLevel()
{
  return m_currentLevel;
}

void pumpController::hardStop()
{
  //Serial.print("in hardstop\n");
  setEnVal(0);
}

void pumpController::setPumpLevel(float val)
{
  // the val is between 0 and 1 (min to max)
  int pwm_level;
  m_calibration.getPWMFromScaledSpoutHeight(val,pwm_level);
  int adjusted_pwm_level = pwm_level*m_heightScaleFactor;
  if(adjusted_pwm_level>255.0) adjusted_pwm_level=255;
  if(adjusted_pwm_level<0.0) adjusted_pwm_level=0;
  if(setEnVal(adjusted_pwm_level))
    m_currentLevel = val;
#if 0
  Serial.print(m_name);
  Serial.print(": adjusted_pwm = ");
  Serial.print(adjusted_pwm_level);
  Serial.print("\n");
#endif  
}

void pumpController::setInLoPin(int val)
{
  m_inLoPin=val;
}

void pumpController::setInHiPin(int val)
{
  m_inHiPin=val;
}

bool pumpController::setEnPin(int val)
{
  m_enPin = val;
}

bool pumpController::setEnVal(int val)
{
#if 0
  Serial.print("Sending PWM val = ");
  Serial.print(val);
  Serial.print(" to pin ");
  Serial.print(m_enPin);
  Serial.print("\n");
#endif  
  if(val<0)
    {
      printMessage("pwm value less than zero");
      return false;
    }
  if(val>255)
    {
      printMessage("pwm value greater than 255");      
      return false;
    }
  
  analogWrite(m_enPin, val); // Send PWM signal to L298N Enable pin
  return true;
}

void pumpController::setName(char *s)
{
  sprintf(m_name,"%s",s);
}

void pumpController::setDebugLevel(int val)
{
  m_debugLevel = val;
}

void pumpController::setHeightScaleFactor(float val)
{
  m_heightScaleFactor = val;
}

void pumpController::printMessage(char *s)
{
  char message[1000];
  sprintf(message,"%s: %s\n",m_name,s);
  Serial.println(s);
}


void pumpController::printPumpInfo()
{
  char line[1000];
  Serial.print("pump_name = ");
  Serial.print(m_name);
  Serial.print(" : inLoPin = ");
  Serial.print(m_inLoPin);
  Serial.print(" : inHiPin = ");
  Serial.print(m_inHiPin);
  Serial.print("\n");

  Serial.print("    Calibration:\n");
  m_calibration.printCalibration();
}
