#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ilGabbibbo";
const char* password = "P4p3r1ss1m4";

const char *serviceURI = "http://localhost:8000";//ip del pc per passare da uvicorn

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


void loop() {
  if(WiFi.status()== WL_CONNECTED){      
    HTTPClient http;
  
    String servicePath = String(serviceURI) + "/api/data";
    
    http.begin(servicePath);
      
    // Send HTTP GET request
    int httpResponseCode = http.GET();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    // Free resources
    http.end();

    delay(2000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    connectToWifi(ssid, password);
  }
}
