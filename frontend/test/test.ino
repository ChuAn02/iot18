#include "led.h"
#include <WiFi.h> 
#include <WebServer.h> 
#include <ArduinoJson.h> 
#include <WebSocketsServer.h> 

const int ledPin = 5;
const int lightSensorPin = 4;
const char* ssid = "P201";
const char* password = "0338959598";

String web="";
WebServer server(80);  
WebSocketsServer webSocket(81);
Led led(ledPin);
void setup() {
  Serial.begin(115200);
  wifiSetup();
  serverSetup();
  led.begin();
}
void wifiSetup(){
  WiFi.begin(ssid,password);
  while(!WiFi.isConnected()){
    delay(500);
  }
  Serial.print("\nIP address:");
  Serial.println(WiFi.localIP());
}
void serverSetup(){
  server.on("/",[](){
    server.send(200,"text\html",web);
  })
}
void loop() {
  // //led.turnOn();
  //   for(float i=0;i<1;i+=0.1){
  //     led.changeBrightness(i);
  //     Serial.println((int)(i*255));
  // // if (ledState == HIGH) led.turnOff();
  // // else {
  // // }
  // delay(1000);}
  // led.turnOff();

  // int ledState = digitalRead(5);
  // Serial.println(ledState);
  // delay(5000);
  // // increase the LED brightness
  // for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
  //   // changing the LED brightness with PWM
  //   ledcWrite(ledChannel, dutyCycle);
  //   delay(15);
  // }

  // // decrease the LED brightness
  // for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
  //   // changing the LED brightness with PWM
  //   ledcWrite(ledChannel, dutyCycle);
  //   delay(15);
  // }
}