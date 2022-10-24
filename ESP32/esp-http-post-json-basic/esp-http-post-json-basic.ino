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

const char *serviceURI = "http://localhost:8000/";


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
  String mgs_json = "{ \"temperature\": " + String(tmp) + ", \"light\": " + String(light) +" }";
  Serial.println(address + " -> " + mgs_json);
  int retCode = http.POST(mgs_json);
  int retcode2 = http.GET();
  http.end();  
  Serial.print("ERRORI PAZZI SGRAVATI -> POST: ");
  Serial.print(retCode);
  Serial.print(" | GET: ");
  Serial.print(retcode2);
  Serial.println();
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

void loop() {
  if (WiFi.status()== WL_CONNECTED){
    int temp = random(15,20);
    int light = random(1, 10);
    int code = sendData(String(serviceURI)+"items/", temp, light);

    Serial.print("CODE DELLA RICHIESTA ");
    Serial.println(code);//*/
    
    int code2 = receiveData(String(serviceURI) + "api/data/3");
    Serial.print("CODE DELLA RICHIESTA ");
    Serial.println(code2);
    Serial.print("Address: ");
    Serial.println(serviceURI);
    Serial.print("Address string: ");
    Serial.println(String(serviceURI));//*/
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
