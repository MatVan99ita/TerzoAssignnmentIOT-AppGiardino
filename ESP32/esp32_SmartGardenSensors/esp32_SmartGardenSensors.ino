/*
 * HTTPClient lib --  Performing an HTTP POST to our REST service
 *
 * Remark:
 * - Going through ngrok
 *
 */
/*#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>//*/
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DEBUG 0
#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define DHT11PIN 32
#define PHOTO_PIN 34 //A7
#define LED_PIN 26
#define DHT_TYPE DHT11

const char *ssid = "ilGabbibbo";
const char *password = "P4p3r1ss1m4";

const char *serviceURI = "http://192.168.221.158:8000/";

int lightInit = 0;
String status = "";

DHT dht(DHT11PIN, DHT_TYPE);

void connectToWifi(const char* ssid, const char* password){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}


int sendData(String address, int tmp, int light){
  WiFiClient client;
  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.begin(client, address + "esp/both/" + String(tmp) + "&" + String(light));
  http.begin(address + "esp/both/" + String(tmp) + "&" + String(light));
  int retCode = http.POST("");
  if(retCode > 0){
    Serial.print(String(retCode));
  } else {
    Serial.println(http.errorToString(retCode));
  }
  http.end();
  return retCode;
}

int receiveData(String address){
  WiFiClient client;
  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.begin(client, address+"arduino/status/");
  http.begin(address+"arduino/status/");
  int retCode = http.GET();
  if(retCode > 0){
    Serial.print(String(retCode));
    String payload = http.getString();
    Serial.print("Payload - ");
    Serial.println(payload);
    status = payload;
    if(status == "\"ERROR\""){
      digitalWrite(LED_PIN, LOW);
    } else {
      digitalWrite(LED_PIN, HIGH);
    } 
  } else {
    Serial.println(http.errorToString(retCode));
  }
  
  http.end(); 
  return retCode;
}

int readLight(){
  float l = analogRead(PHOTO_PIN);
  return map(l, 0, 4095, 7, 0);
}

int readTemperature(){
  float temp = dht.readTemperature();
  return map(temp, 0, 40, 0, 4);
}

void setup() {
  Serial.begin(115200);
  //analogReadResolution(12);
#if DEBUG == 0
  pinMode(LED_PIN, OUTPUT);
  pinMode(PHOTO_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
  pinMode(DHT11PIN, INPUT);
#endif
  connectToWifi(ssid, password);
}

void loop() {
  int temp;
  int light;
  if (WiFi.status()== WL_CONNECTED){
#if DEBUG == 1
    light = map(random(0, 4095), 0, 4095, 0, 7);
    temp = map(random(0, 4095), 0, 4095, 0, 4);
#else
    temp = readTemperature();
    light = readLight();
    Serial.print("BOTH");
    Serial.print(temp);
    Serial.print(" ");
    Serial.print(dht.readTemperature());
    Serial.print(" & ");
    Serial.print(light);
    Serial.print(" ");
    Serial.println(analogRead(PHOTO_PIN));

#endif
    int code = sendData(String(serviceURI), temp, light);
    if (code == 200){
      Serial.println("ok");
    } else {
      Serial.println(String("error: ") + code);
    }
    code = receiveData(String(serviceURI));
    if (code == 200){
      Serial.println("ok");
    } else {
      Serial.println(String("error: ") + code);
    }
    delay(1000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    connectToWifi(ssid, password);
  }
}
