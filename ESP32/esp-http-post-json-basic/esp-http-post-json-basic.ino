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
#define DHT11PIN A4
#define PHOTO_PIN 35 //A7
#define LED_PIN 26

const char *ssid = "ilGabbibbo";
const char *password = "P4p3r1ss1m4";

const char *serviceURI = "http://192.168.72.158:8000/";

int lightInit = 0;
String status = "";

DHT dht(DHT11PIN, DHT11);

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


int sendData(String address, float tmp, float light){
  WiFiClient client;
  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  http.begin(client, address + "esp/both/" + String(tmp) + "&" + String(light));
  http.begin(address + "esp/both/" + String(tmp) + "&" + String(light));
  int retCode = http.POST("");
  Serial.print(String(retCode));
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
  http.end();  
  return retCode;
}

int readLight(){
  return map(analogRead(PHOTO_PIN), 0, 4095, 0, 8);
}

int readTemperature(){
  // read the ADC value from the temperature sensor
  int adcVal = analogRead(LM35_PIN);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);//3300.0
//4096.0
  // convert the voltage to the temperature in °C
  float tempC = milliVolt / 10;
  // convert the °C to °F
  //float tempF = tempC * 9 / 5 + 32;
  //NON HO CAPITO SE VA CONVERTITO COMUNQUE O SE POSSO MAPPARE DIRETTAMENTE L'INPUT e quindi mapp l'input
  return map(analogRead(LM35_PIN), 0, 4095, 0, 5);
}

void setup() {
  Serial.begin(115200);
  //analogReadResolution(12);
#if DEBUG == 0
  pinMode(LED_PIN, OUTPUT);
  pinMode(PHOTO_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();
#endif
  connectToWifi(ssid, password);
}

void loop() {
  int temp;
  int light;
  if (WiFi.status()== WL_CONNECTED){


#if DEBUG == 1
    int t1 = random(0, 4095);
    light = map(random(0, 4095), 0, 4095, 0, 8);
    int t2 = random(0, 4095);
    temp = map(random(0, 4095), 0, 4095, 0, 5);
    Serial.println("LUCE: "+String(t1)+" -> "+String(light));
    Serial.println("TEMPERATURA: "+String(t2)+" -> "+String(temp));
#else
    temp = dht.readTemperature();
    light = readLight();
    Serial.println("LUCE: "+String(light));
    Serial.println("TEMPERATURA: "+String(temp));
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
