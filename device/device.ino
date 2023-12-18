#include "led.h"
#include "fan.h"
#include "DHTesp.h"
#include "time.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <string>

const char* ssid = "P201";
const char* password = "0338959598";
const char* mqttUsername = "device";
const char* mqttPassword = "012345Aa";
const char* mqttServer = "8e4fc9a27eb74ad9a56752fb668f38ba.s1.eu.hivemq.cloud";
const int mqttPort = 8883;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = +7 * 60 * 60;
const int daylightOffset_sec = 0;

const int ledPin = 4;
const int lightSensorPin = 5;
const int fanPin = 23;
const int DHTPin = 16;
long int lastTime = 0;
long int delayTime = 5000;

Led led;
Fan fan;
DHTesp dhtSensor;
WiFiClientSecure espClient;
PubSubClient client(espClient);
float temp;
float hum;

void saveData() {
  client.publish("/light/save", String(led.getCountTimeAndReset() / 3600).c_str());
  client.publish("/fan/save", String(fan.getCountTimeAndReset() / 3600).c_str());
}
void checkFanAuto() {
  if (fan.isEnableAuto()) {
    bool b = fan.getMaxTemp() <= temp && fan.getMaxHum() <= hum;
    if (fan.isOn() != b) {
      if (b) fan.autoTurnOn();
      else fan.autoTurnOff();
      client.publish("/info/fan", fan.toJson().c_str());
    }
  }
}

void checkLedAuto() {
  if (led.isEnableAuto()) {
    bool lightSensorState = digitalRead(lightSensorPin) == HIGH;
    if (lightSensorState != led.isOn()) {
      if (lightSensorState) led.autoTurnOn();
      else led.autoTurnOff();
      client.publish("/info/light", led.toJson().c_str());
    }
  }
}

void checkLedSchedulde() {
  if (led.isEnableSchedule()) {
    bool ch = led.checkSchedule();
    if (ch != led.isOn()) {
      if (ch) led.autoTurnOn();
      else led.autoTurnOff();
      client.publish("/info/light", led.toJson().c_str());
    }
  }
}

void autoPublish() {
  hum = 1.0 * (int)(dhtSensor.getHumidity() * 100) / 100;
  temp = 1.0 * (int)(dhtSensor.getTemperature() * 100) / 100;
  String temphumJson = "{\"temp\":" + String(temp) + ", \"hum\":" + String(hum) + "}";
  client.publish("/info/temphum", temphumJson.c_str());
  // Serial.println(temphumJson);
}

void subscribe() {
  client.subscribe("/info");
  //light
  client.subscribe("/light/status");
  client.subscribe("/light/schedule");
  client.subscribe("/light/brightness");
  client.subscribe("/light/auto/status");
  client.subscribe("/light/schedule/status");
  //fan
  client.subscribe("/fan/status");
  client.subscribe("/fan/level");
  client.subscribe("/fan/auto/status");
  client.subscribe("/fan/setting");
}

void handle(String topic, String message) {
  if (topic == "/info") {
    if (message == "light")
      client.publish("/info/light", led.toJson().c_str());
    else if (message == "fan")
      client.publish("/info/fan", fan.toJson().c_str());
    else if (message == "temphum") {
      String str = "{\"temp\":" + String(temp) + ", \"hum\":" + String(hum) + "}";
      client.publish("/info/temphum", str.c_str());
    }
    return;
  }
  //
  if (topic == "/light/status") {
    if (message == "on") {
      led.turnOn();
    } else led.turnOff();
  } else if (topic == "/light/auto/status") {
    led.enableAuto(message == "on");
  } else if (topic == "/light/schedule/status") {
    led.enableSchedule(message == "on");
  } else if (topic == "/light/brightness") {
    int lv = message.toInt();
    led.changeBrightness(lv);
  } else if (topic == "/light/schedule") {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, message);
    String startTime = doc["startTime"];
    String endTime = doc["endTime"];
    int day[8], ind = 0;
    JsonArray array = doc["day"].as<JsonArray>();
    for (JsonVariant v : array) {
      day[ind++] = v.as<int>();
    }
    led.setSchedule(startTime, endTime, day);
  }
  //
  else if (topic == "/fan/status") {
    if (message == "on") {
      fan.turnOn();
    } else fan.turnOff();
  } else if (topic == "/fan/level") {
    int lv = message.toInt();
    fan.changeLevel(lv);
  } else if (topic == "/fan/auto/status") {
    fan.enableAuto(message == "on");
  } else if (topic == "/fan/setting") {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, message);
    int maxTemp = doc["maxTemp"];
    int maxHum = doc["maxHum"];
    fan.setting(maxTemp, maxHum);
  }
  client.publish("/info/light", led.toJson().c_str());
  client.publish("/info/fan", fan.toJson().c_str());
}

void callback(char* charArrTopic, byte* payload, unsigned int length) {
  String message;
  String topic = String(charArrTopic);
  for (int i = 0; i < length; i++)
    message += (char)payload[i];
  Serial.println("Message: [  topic = " + topic + " , msg = " + message + "  ]");
  handle(topic, message);
}

void reconnect() {
  String clientId = "Esp32 - device";
  Serial.print("Connect to server..");
  while (!client.connected()) {
    Serial.print(".");
    if (client.connect(clientId.c_str(), mqttUsername, mqttPassword)) {
      Serial.println("\nConnect to server successful!");
      subscribe();
    } else delay(5000);
  }
}

void wifiSetup() {
  WiFi.begin(ssid, password);
  Serial.print("Connect to WiFi ..");
  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());
}

//
void setup() {
  delay(1000);
  Serial.begin(115200);
  espClient.setInsecure();
  wifiSetup();
  led.setup(ledPin);
  fan.setup(fanPin);
  dhtSensor.setup(DHTPin, DHTesp::DHT11);
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  checkFanAuto();
  checkLedAuto();
  checkLedSchedulde();
  unsigned long now = millis();
  if (now - lastTime >= delayTime) {
    lastTime = now;
    autoPublish();
  }
  struct tm current;
  getLocalTime(&current);
  if (current.tm_hour==23 && current.tm_min==59 && current.tm_sec <= 1) {
    saveData();
    delay(2000);
  }
}