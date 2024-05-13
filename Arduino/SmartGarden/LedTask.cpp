#include "LedTask.h"
#include "Arduino.h"

#define PIN_LED_SWITCH1 12
# define PIN_LED_SWITCH2 11
#define PIN_LED_FADE1 10
#define PIN_LED_FADE2 9

LedTask::LedTask(){
    led_switch1 = new Led(PIN_LED_SWITCH1)
    led_switch2 = new Led(PIN_LED_SWITCH2)
    led_fade1 = new Led(PIN_LED_FADE1)
    led_fade2 = new Led(PIN_LED_FADE2)
};


void LedTask::init(int period){
    Task::init(period)
    state = CHANGE_ILLUMINATION_STATUS
}

void LedTask::tick(){
    switch (state)
    {
    case CHANGE_ILLUMINATION_STATUS:
        //TODO: agginta controlli per far partire i led
        if(led_type=="SWITCH")
        {
          if(led_id==1){
            led_switch1->change();
          } else if(led_id==2) {
            led_switch2->change();
          }
        } else if(led_type == "FADE") {
          if(led_id==1){
            led_fade1->fade();
          } else if (led_id==2) {
            led_fade2->fade();        
          } else if (led_id==3) {
            led_fade1->fade();
            led_fade2->fade();
          }
        }
        //Terminazione task
        this->setActive(false);
        break;
    }
}

