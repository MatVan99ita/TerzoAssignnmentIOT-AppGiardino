/*
 * HTTPClient lib -- simple HTTP GET
 */
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ErCattecolling";
const char* password = "Porc0D10";

const char *serverPath = "https://b92c-137-204-20-123.eu.ngrok.io/";

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
  
    // Your Domain name with URL path or IP address with path
    http.begin(serverPath);
    
    // Send HTTP GET request
    int httpResponseCode = http.GET();
      
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
