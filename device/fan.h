#ifndef fan_h
#define fan_h
#include "Arduino.h"

class Fan {
public:
  Fan();
  void setup(int pin, int channel = 1);
  void turnOn();
  void turnOff();
  void autoTurnOn();
  void autoTurnOff();
  void enableAuto(bool aut);
  void setting(int maxTemp, int maxHum);
  bool isEnableAuto();
  void changeLevel(int val);
  int getMaxTemp();
  int getMaxHum();
  bool isOn();
  float getCountTimeAndReset();
  String toJson();
private:
  int _pin;
  int _channel;
  bool _on;
  bool _auto;
  int _maxTemp = 60;
  int _maxHum = 100;
  float _countTime = 0;
  float _turnonTime;
  float _val;
  const float maxVal = 3;
  const int freq = 30000;
  const int resolution = 8;
};

#endif