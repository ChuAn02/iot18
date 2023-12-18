#include "esp32-hal-ledc.h"
#include "esp32-hal-gpio.h"
#include "Arduino.h"
#include "led.h"

Led::Led(int pin,int channel){
  _pin=pin;
  _channel=channel;
}
void Led::begin(){
  pinMode(_pin,OUTPUT);
  ledcSetup(_channel, freq, resolution);
  ledcAttachPin(_pin, _channel);
}
void Led::turnOff(){
  digitalWrite(_pin,LOW);
}
void Led::turnOn(){
  digitalWrite(_pin,HIGH);
}
void Led::changeBrightness(float val){
  ledcWrite(_channel, (int)(val*255));
}