#ifndef led_h
#define led_h
#include "Arduino.h"
#include "schedule.h"

class Led {
public:
  Led();
  void setup(int pin, int channel = 0);
  bool isOn();
  void turnOn();
  void turnOff();
  void autoTurnOn();
  void autoTurnOff();
  void enableAuto(bool aut);
  bool isEnableAuto();
  void enableSchedule(bool sch);
  bool isEnableSchedule();
  void changeBrightness(int val);
  bool checkSchedule();
  void setSchedule(String startTime,String endTime,int day[]);
  float getCountTimeAndReset();
  String toJson();
private:
  Schedule schedule;
  int _pin;
  int _channel;
  float _val;
  bool _on;
  bool _auto;
  bool _isSche;
  float _countTime=0;
  float _turnonTime;
  const float maxVal=10;
  const int freq = 5000;
  const int resolution = 8;
};

#endif