#include "IlluminationTask.h"
#include "Arduino.h"
#include "Config.h"

IlluminationTask::IlluminationTask(){
  led_s1 = new Led_switch( L1_SWITCH );
  led_s2 = new Led_switch( L2_SWITCH );
  led_f1 = new Led_fade( L1_FADE );
  led_f2 = new Led_fade( L2_FADE );
}
  
void IlluminationTask::init(int period){
  Task::init(period);
  state = CHANGE_ILLUMINATION;    
}

void IlluminationTask::tick(){
  switch (state){
    case CHANGE_ILLUMINATION:
    Serial.println("ILLUMINA!");
      if(led_type=="L")
      {
        if(led_id==1){
          led_s1->change();
        }else{
          led_s2->change();
        }
      }else{
        if(led_id==1){
          led_f1->fade(value);
        }else if (led_id==2){
          led_f2->fade(value);
        } else if (led_id==3) {
          led_f1->fade(value);
          led_f2->fade(value);
        }
      }
      this->setActive(false);
      break;
  }
}
