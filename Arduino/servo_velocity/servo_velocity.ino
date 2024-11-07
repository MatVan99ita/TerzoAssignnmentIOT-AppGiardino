

// include the Servo library
#include <Servo.h>

Servo motor;  // create a servo object


void setup() {
  // put your setup code here, to run once:
motor.attach(7);
}

void loop() {
  // put your main code here, to run repeatedly:
int dilei = 15;
int i = 0;
    for(i = 0; i < 180; i+= 1) {
        motor.write(750+(i*10));
        delay(dilei);
    }
    delay(dilei);
    for(int i = 180; i >= 0; i-= 1) {
        motor.write(750+(i*10));
        delay(dilei);
    }
    motor.detach();

//O ANCHE COSI'
  myServo.write(angle);  // Imposta l'angolo del servomotore
  delay(delayTime);      // Aspetta un po' per vedere il movimento
  
  // Aggiorna l'angolo
  angle += step;

  // Inverte la direzione quando raggiunge i limiti
  if (angle >= 180 || angle <= 0) {
    step = -step;
  }
}