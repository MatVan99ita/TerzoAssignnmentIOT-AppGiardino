/* 
HC05 - Bluetooth AT-Command mode 
modified on 10 Feb 2019 
by Saeed Hosseini 
https://electropeak.com/learn/guides
*/ 
#include "SoftwareSerial.h"
SoftwareSerial MyBlue(3, 2); // RX | TX 
void setup() 
{ 
  Serial.begin(9600); 
  MyBlue.begin(9600);  //Baud Rate for AT-command Mode.  
  Serial.println("***AT commands mode***"); 
} 
void loop() 
{ 
  //from bluetooth to Terminal. 
  if (MyBlue.available()) 
    Serial.write(MyBlue.read()); 
  //from termial to bluetooth 
  if (Serial.available()) {
    MyBlue.write("ALLARMO"/*Serial.read()*/);
  }
}
