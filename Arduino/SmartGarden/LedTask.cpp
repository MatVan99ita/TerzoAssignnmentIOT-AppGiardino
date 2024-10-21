#include "LedTask.h"
#include "Arduino.h"
#include "Setting.h"

#define PIN_LED_SWITCH1 12
#define PIN_LED_SWITCH2 11
#define PIN_LED_FADE1 10
#define PIN_LED_FADE2 9

LedTask::LedTask(){
    led_s1 = new Led(PIN_LED_SWITCH1);
    led_s2 = new Led(PIN_LED_SWITCH2);
    led_f1 = new Led(PIN_LED_FADE1);
    led_f2 = new Led(PIN_LED_FADE2);
};


void LedTask::init(int period){
    Task::init(period);
    this->led_id = led_id;
}

void LedTask::tick(){
    if(led_type == "LEDAUTO") {
      //Automatic light with 2 blink and 2 fade with temperature value
      led_s1->change();
      led_s2->change();
      led_f1->fade(servo_speed);
      led_f2->fade(servo_speed);
      
    } else if(led_type == "LEDMANUAL"){
      if(led_type == "LEDB")
      {
        if(led_id == 1){
          led_s1->change();
        } else if(led_id == 2) {
          led_s2->change();
        } else if (led_id == 3) {
          led_s1->change();
          led_s2->change();
        }
      } else if(led_type == "LEDF") {
        if(led_id==1){
          led_f1->fade(servo_speed);
        } else if (led_id==2) {
          led_f2->fade(servo_speed);        
        } else if (led_id==3) {
          led_f1->fade(servo_speed);
          led_f2->fade(servo_speed);
        }
      }
    }
  
  
  //Terminazione task
  this->setActive(false);
}
