#include "ledController.h"

ledController::ledController()
{

}



ledController::~ledController()
{

}
#define DUMB_PIN 10
CRGB dumb_leds[10];
void ledController::setNumLEDs(int num_leds)
{
  m_numLEDs = num_leds;
}

void ledController::setControlPin(int control_pin)
{
  m_controlPin=control_pin;
}

void ledController::setupLED(char *name,int num_leds)
{
  setName(name);
  m_numLEDs = num_leds;

  // set master brightness control
  FastLED.setBrightness(MAX_BRIGHTNESS);

  // insert a delay to keep the framerate modest
  FastLED.delay(m_timeDelay_ms);   

  m_initialized=true;
}

void ledController::setName(char *s)
{
  sprintf(m_name,"%s",s);
}

void ledController::setDebugLevel(int val)
{
  m_debugLevel = val;
}

void ledController::printMessage(char *s)
{
  char message[1000];
  sprintf(message,"%s: %s\n",m_name,s);
  Serial.println(s);
}
