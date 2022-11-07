
#include <Servo.h>
#define SERVO_PIN 9

Servo myservo;
int mapSpeed;
int pos = 180;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveto(60, 30);
}

int pos1 = 0;
void moveto(int position, int speed){
  if(speed < 0) speed = 0;
  else if(speed > 30) speed = 30;
  
  mapSpeed = map(speed, 0, 30, 30, 0);

  for (pos = 0; pos <= 180; pos += 1) {     // goes from 0 degrees to 180 degrees
    myservo.write(pos);                     // tell servo to go to position in variable 'pos'
    delay(mapSpeed);                        // waits n ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) {     // goes from 180 degrees to 0 degrees
    myservo.write(pos);                     // tell servo to go to position in variable 'pos'
    delay(mapSpeed);                        // waits n ms for the servo to reach the position
  }
  
}
