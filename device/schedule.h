#ifndef schedule_h
#define schedule_h
#include "Arduino.h"

class Schedule {
public:
  Schedule();
  void setTime(String startTime, String endTime, int day[]);
  bool checkTime();
  String toJson();
private:
  String _startTime="00:00";
  String _endTime="00:00";
  int _day[8]={0};
};

#endif