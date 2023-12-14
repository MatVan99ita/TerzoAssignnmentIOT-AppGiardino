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
  
  Serial.begin(115200);
  Serial.setTimeout(1);
  bt.begin(9600);
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

ManualMode(){
  //Check segnale bluetooth
  //Se non c'è chissene
  //Se c'è bloccare l'auto mode e rendere 
  //attivabili in modo manuale le task
  mode = 1
  if(/*fine manual*/){
    mode = 0
  }
}


  CheckManualSignal(){
    while(bt.available() == 0);

    if(bt.available() > 0) data = bt.parseInt();
    
    delay(400);

    if(data == '1') mode =  1
  }


void setup(){
  // put your setup code here, to run once:
  myservo.attach(9);
  myservo.write(0);
  Mode=Auto;
  }

void loop{
  //attende un eventuale messaggio dall'app per modalità manuale
  switch (mode)
  {
  case 0:
    CheckLight();
    CheckTemp();
    CheckManualSignal();
    delay(10000);
    break;
  case 1:
    /* MANUAL */
    if(bt.available() > 0) data = bt.parseInt();
    //data sarà una stringa hardocoddata con i comandi che vengono mandati
    if(data.substr()=="") //esegui comando
    break;
  case 2:
    /* ALARM */
    break;
  
  default:
    break;
  }
  
  if(mode==0){
  CheckLight();
  CheckTemp();
  CheckManualSignal();
  delay(10000);
  } else if(mode == 2){
    //ALARM
  } else {}

}
