#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H


#include <Arduino.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define MAX_BRIGHTNESS 255

class ledController
{
 public:
  ledController();
  ~ledController();
  void setupLED(char *name,int num_leds);
  void setNumLEDs(int num_leds);
  void setControlPin(int control_pin);
  void setName(char *s);
  void setDebugLevel(int val);
  void printMessage(char *s);
  
  bool m_use = true;
  bool m_initialized = false;
  uint8_t m_currentRed = 0;
  uint8_t m_currentGreen = 0;
  uint8_t m_currentBlue = 0;  

 private:
  uint8_t m_controlPin;
  uint8_t m_numLEDs;
  CRGB *m_ledArray;
  uint8_t m_id;
  char m_name[20];
  // 0 is no debug, greater is more debug
  uint8_t m_debugLevel = 0;
  int m_timeDelay_ms = 10;
};

#endif
