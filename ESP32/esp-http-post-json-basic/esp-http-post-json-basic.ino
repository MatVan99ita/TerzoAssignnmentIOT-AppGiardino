/*
 * HTTPClient lib --  Performing an HTTP POST to our REST service
 *
 * Remark:
 * - Going through ngrok
 *
 */
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "ilGabbibbo";
const char *password = "P4p3r1ss1m4";

const char *serviceURI = "192.168.198.158:53000";


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

int sendData(String address, float value, String place){  
  
   HTTPClient http;    
   http.begin(String(address));
   http.addHeader("Content-Type", "application/json");
    
   String msg = 
    String("{ \"value\": ") + String(value) + 
    ", \"place\": \"" + place +"\" }";
   
   int retCode = http.POST(msg);   
   http.end();  

   String payload = http.getString();
   Serial.println(payload);
      
   return retCode;
}

int receiveData(String address){
   HTTPClient http;    
   http.begin(address);
   
   http.addHeader("Content-Type", "application/json");
   
   int retCode = http.GET();
   String payload = http.getString();
   Serial.print("Payload XD ");
   Serial.println(payload);
      
   http.end();  
   return retCode;
}

void loop() {
  if (WiFi.status()== WL_CONNECTED){      

    int value = random(15,20);
    int code = sendData(String(serviceURI), value, "banana");
    Serial.print("CODE DELLA RICHIESTA ");
    Serial.println(code);
    Serial.print("Address: ");
    Serial.println(serviceURI);
    Serial.print("Address string: ");
    Serial.println(String(serviceURI));
    
    code = receiveData(String(serviceURI));
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
