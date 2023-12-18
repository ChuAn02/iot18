#include "esp32-hal-ledc.h"
#include "esp32-hal-gpio.h"
#include "Arduino.h"
#include "led.h"
#include "schedule.h"
#include "time.h"

Led::Led() {}

void Led::setup(int pin, int channel) {
  _pin = pin;
  _channel = channel;
  _val = maxVal;
  _on = false;
  _auto = false;
  _isSche = false;
  pinMode(_pin, OUTPUT);
  ledcSetup(_channel, freq, resolution);
  ledcAttachPin(_pin, _channel);
}
bool Led::isOn() {
  return _on;
}
void Led::turnOff() {
  _auto = false;
  _isSche = false;
  autoTurnOff();
}

void Led::turnOn() {
  _auto = false;
  _isSche = false;
  autoTurnOn();
}

void Led::autoTurnOn() {
  _on = true;
  ledcWrite(_channel, (int)(_val / maxVal * 255));
  struct tm current;
  getLocalTime(&current);
  _turnonTime = current.tm_hour * 60 * 60 + current.tm_min * 60 + current.tm_sec;
}

void Led::autoTurnOff() {
  _on = false;
  ledcWrite(_channel, 0);
  struct tm current;
  getLocalTime(&current);
  _countTime += current.tm_hour * 60 * 60 + current.tm_min * 60 + current.tm_sec - _turnonTime;
}

void Led::changeBrightness(int val) {
  _val = val;
  if (!_on) return;
  ledcWrite(_channel, (int)(_val / maxVal * 255));
}

bool Led::isEnableAuto() {
  return _auto;
}

bool Led::isEnableSchedule() {
  return _isSche;
}

void Led::enableAuto(bool aut) {
  _auto = aut;
  _isSche = false;
}

void Led::enableSchedule(bool sch) {
  _isSche = sch;
  _auto = false;
}

bool Led::checkSchedule() {
  return schedule.checkTime();
}

void Led::setSchedule(String startTime, String endTime, int day[]) {
  schedule.setTime(startTime, endTime, day);
}

float Led::getCountTimeAndReset() {
  float countTime = _countTime;
  _countTime = 0;
  if (_on) {
    struct tm current;
    getLocalTime(&current);
    float currentTime = current.tm_hour * 60 * 60 + current.tm_min * 60 + current.tm_sec;
    countTime += currentTime - _turnonTime;
    _turnonTime = currentTime;
  }
  return countTime;
}

String Led::toJson() {
  String msg = "{";
  msg += "\"status\":" + String(_on);
  msg += ", \"brightness\":" + String((int)_val);
  msg += ", \"auto\":" + String(_auto);
  msg += ", \"schedule\":" + String(_isSche);
  msg += schedule.toJson();
  msg += "}";
  return msg;
}