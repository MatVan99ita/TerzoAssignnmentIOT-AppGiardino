#include "IrrigationTask.h"
#include "Arduino.h"
#include "Setting.h"

#define SERVO_PIN 6

IrrigationTask::IrrigationTask(){
    this -> servo = new Servo(SERVO_PIN);
}

void IrrigationTask::init(int period){
    Task::init(period);
}

/*
Dall'esempio del blink-print task ogjni 500 millisecondi partono entrambe
appena finisce  quella più lunga(cioè il print allungato) si riattiva la task led
Dunque con il servo servirà una volta partita la task che si accenda, faccia le sue cose e si spenga
però nel fare le sue cose lo deve fare per Y secondi -> questi Y secondi sono dettati dalla velocità 
del servo che più fa caldo più dovrebbe andare in teoria mentre per il timing basta tenerlo fisso perchè 
quello è la temporizzazzione che ogni secondo lo va a ricontrollare se è attivo e poi funziona per il periodo che gli è impostato in secondo tempo
Il blocco di X minuti lo si mette a monte per evitare casini
*/
void IrrigationTask::tick(){
  //Irrigation task start
  this -> servo->on();// attach the pin
  this -> servo->setSpeed(servo_speed);// set the speed
  this -> servo->startRotation(); // body of the task
  this -> servo->off(); // detach the servo
  this -> setActive(false);  //Irrigation task end
}
