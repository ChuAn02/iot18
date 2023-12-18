#include "Arduino.h"
#include "schedule.h"
#include "time.h"

Schedule::Schedule() {}

void Schedule::setTime(String startTime, String endTime, int day[]) {
  _startTime = startTime;
  _endTime = endTime;
  for (int i = 0; i < 8; i++) _day[i] = day[i];
}

bool Schedule::checkTime() {
  struct tm current;
  if (!getLocalTime(&current)) {
    Serial.println("Failed to obtain time");
    return false;
  }
  if (_day[current.tm_wday + 1]) {
    int curTime = current.tm_hour * 60 + current.tm_min;
    int stTime = _startTime.substring(0, 2).toInt() * 60 + _startTime.substring(3, 5).toInt();
    int enTime = _endTime.substring(0, 2).toInt() * 60 + _endTime.substring(3, 5).toInt();
    if (curTime >= stTime && curTime < enTime) return true;
  }
  return false;
}

String Schedule::toJson() {
  String msg = "";
  msg += ", \"startTime\": \"" + _startTime + "\"";
  msg += ", \"endTime\": \"" + _endTime + "\"";
  msg += ", \"day\":[";
  for (int i = 0; i < 8; i++) msg += String(_day[i]) + ",";
  msg.setCharAt(msg.length() - 1, ']');
  return msg;
}