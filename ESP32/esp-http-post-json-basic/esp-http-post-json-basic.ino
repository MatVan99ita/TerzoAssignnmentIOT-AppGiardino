/*
 * HTTPClient lib --  Performing an HTTP POST to our REST service
 *
 * Remark:
 * - Going through ngrok
 *
 */
#include <WiFi.h>
#include <HTTPClient.h>//*/
/*#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>//*/
#include <ArduinoJson.h>

const char *ssid = "ilGabbibbo";
const char *password = "P4p3r1ss1m4";

const char *serviceURI = "http://192.168.198.158:8000/";


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

void setup() {
  Serial.begin(115200); 
  connectToWifi(ssid, password);
}

int sendData(String address, float tmp, float light){  
  
  WiFiClient client;
  HTTPClient http;
  http.addHeader("Content-Type", "application/json");
  bool test = http.begin(address);
  if(test){
    Serial.println("Si è connesso a " + address);
  } else {
    Serial.println("attaccati al casso");
  }

  // Create the "analog" array
  
  Serial.println();
  String mgs_json = "{ \"temperatura\": " + String(tmp) + ", \"lux\": " + String(light) +" }";
  String msg = "esp/both/" + String(tmp) + "&" + String(light);
  Serial.println(address + " -> " + mgs_json);
  int retCode = http.POST(mgs_json);
  http.end();
  return retCode;
}

int receiveData(String address){
  WiFiClient client;
  HTTPClient http;    
  http.begin(address);
  
  http.addHeader("Content-Type", "application/json");
  Serial.println("Aggiunti header");
  int retCode = http.GET();
  String payload = http.getString();
  Serial.print("Payload XD ");
  Serial.println(payload);
     
  http.end();  
  return retCode;
}

void loop() { /** MANCA SOLO L'ACCENDI SPEGNI DEL LED PER QUANDO IL SISTEMA NON E' IN ALLARME*/
  if (WiFi.status()== WL_CONNECTED){
    int temp = random(15,20);
    int light = random(1, 10);
    Serial.println(String(temp) + " - " + String(light));
    int code = sendData(String(serviceURI) + "esp/both/" + String(temp) + "&" + String(light), temp, light);
    if (code == 200){
       Serial.println("ok");
     } else {
       Serial.println(String("error: ") + code);
     }    
    delay(5000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    connectToWifi(ssid, password);
  }
}
