#include <Servo.h>
#define SERVO_PIN 9

Servo myservo;
int mapSpeed;
int pos = 0;
int mode = 0;   //0=auto, 1=manuale, 2=ALARM
void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
  myservo.write(0);
}

CheckLight(){
  Light=1//leggi la luce
  if(Light<1000 && luce=0)
    {
    TaskSystem_on();
    }
  else if(Light>2000 && luce=1)
    {
    TaskSystem_off();
    }
  else{
    //scala la luce in base alla luminosità
    luce3;
    luce4;
    }
}

CheckTemp(){
   Temperatura=1; //"readAnalogPin"
   if (Temperatura>Soglia){
      "ALARM Mode activated";
      mode=2;
      //invia un messaggio, blocca tutto
      


      //aspetta il messaggio dall'app
    else if(Temperatura>soglia2){
        //attiva irrigazione, scala la velocità in base alla temperatura
        TaskIrrigatore;
      }
    }
  
  }




void setup(){
  // put your setup code here, to run once:
  myservo.attach(9);
  myservo.write(0);
  Mode=Auto;
  }

void loop{
  //attende un eventuale messaggio dall'app per modalità manuale
  if(mode==0){
  CheckLight();
  CheckTemp();
  delay(10000);
  }

}
