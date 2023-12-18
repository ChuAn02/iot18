#include "esp32-hal-gpio.h"
#include "Arduino.h"
#include "fan.h"

Fan::Fan() {}

void Fan::setup(int pin, int channel) {
  _pin = pin;
  _channel = channel;
  _val = maxVal;
  _on = false;
  _auto = false;
  pinMode(_pin, OUTPUT);
  ledcSetup(_channel, freq, resolution);
  ledcAttachPin(_pin, _channel);
}

bool Fan::isOn() {
  return _on;
}

void Fan::turnOn() {
  _auto = false;
  autoTurnOn();
}

void Fan::turnOff() {
  _auto = false;
  autoTurnOff();
}

void Fan::autoTurnOn() {
  _on = true;
  ledcWrite(_channel, (int)(_val / maxVal * 255));
  struct tm current;
  getLocalTime(&current);
  _turnonTime = current.tm_hour * 60 * 60 + current.tm_min * 60 + current.tm_sec;
}

void Fan::autoTurnOff() {
  _on = false;
  ledcWrite(_channel, 0);
  struct tm current;
  getLocalTime(&current);
  _countTime += current.tm_hour * 60 * 60 + current.tm_min * 60 + current.tm_sec - _turnonTime;
}

void Fan::changeLevel(int val) {
  _val = val;
  if (!_on) return;
  ledcWrite(_channel, 200 + (int)(_val / maxVal * 55));
}

bool Fan::isEnableAuto() {
  return _auto;
}

void Fan::enableAuto(bool aut) {
  _auto = aut;
}

void Fan::setting(int maxTemp, int maxHum) {
  _maxTemp = maxTemp;
  _maxHum = maxHum;
}

int Fan::getMaxHum() {
  return _maxHum;
}

int Fan::getMaxTemp() {
  return _maxTemp;
}

float Fan::getCountTimeAndReset() {
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

String Fan::toJson() {
  String msg = "{";
  msg += "\"status\":" + String(_on);
  msg += ", \"level\":" + String((int)_val);
  msg += ", \"auto\":" + String(_auto);
  msg += ", \"maxTemp\":" + String(_maxTemp);
  msg += ", \"maxHum\":" + String(_maxHum);
  msg += "}";
  return msg;
}