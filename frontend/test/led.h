#ifndef led_h
#define led_h
#include "Arduino.h"

class Led{
  public:
    Led(int pin,int channel=0);
    void begin();
    void turnOn();
    void turnOff();
    void changeBrightness(float val);
  private:
    int _pin;
    int _channel = 0;
    const int freq = 5000;
    const int resolution = 8;
};

#endif