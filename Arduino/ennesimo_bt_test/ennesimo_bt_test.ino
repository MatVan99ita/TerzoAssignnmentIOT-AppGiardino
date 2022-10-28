
#include <SoftwareSerial.h>
SoftwareSerial bt(2,3);

byte data;
char Incoming_value = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  bt.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  while(bt.available() == 0);

  if(bt.available() > 0){
    data = bt.parseInt();
  }
  delay(400);

  if(data == '1')
      digitalWrite(13, HIGH)
    else
      digitalWrite(13. LOW);
  

  
  /*if(Serial.available() > 0){
    Incoming_value = Serial.read();
    Serial.println(Incoming_value);
    if(Incoming_value == '1')
      digitalWrite(13, HIGH)
    else
      digitalWrite(13. LOW);
  }*/
  
}
