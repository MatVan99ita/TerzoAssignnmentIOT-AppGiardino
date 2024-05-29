
#include "DHT.h"

#define DHT11PIN 32

int sensorValue;
int tempValue;

DHT dht(DHT11PIN, DHT11);

void setup()
{
  pinMode(26, OUTPUT);
  Serial.begin(115200); // starts the serial port at 9600
  dht.begin();
  pinMode(DHT11PIN, INPUT);
}
 
void loop()
{
  sensorValue = analogRead(32); // read analog input pin 0
  Serial.print("AAAAAAAAA");
  Serial.println(sensorValue, DEC); // prints the value read
  // prints a space between the numbers

  Serial.println(""); // wait 100ms for next reading
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  tempValue = analogRead(A4);
  if(tempValue >= 20){
    digitalWrite(26, HIGH);
  } else {
    digitalWrite(26, LOW);
  }
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("ºC ");
  Serial.println(map(temp, 0, 40, 0, 7));
  
  Serial.print("Humidity: ");
  Serial.println(humi);
  delay(1000);
}
