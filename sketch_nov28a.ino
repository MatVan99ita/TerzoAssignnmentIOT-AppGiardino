#include <Servo.h>
#include <SoftwareSerial.h>

#define LIGHT1 4
#define LIGHT2 3
#define LIGHT3 5
#define LIGHT4 7
#define TEMPPIN "boh"


Servo myservo;
int pos=0;

SoftwareSerial myblue(2, 3);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
myblue.begin(9600);
myservo.attach(6);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (pos = 0; pos <= 180; pos += 1)
  {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(10);                       // waits 15ms for the servo to reach the position
      Serial.println("Goku");
  }
  for (pos = 180; pos >= 0; pos -= 1)
  {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(10);                       // waits 15ms for the servo to reach the position

  }


  
}
