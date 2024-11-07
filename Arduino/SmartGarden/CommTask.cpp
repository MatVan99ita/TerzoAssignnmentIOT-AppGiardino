#include "CommTask.h"
#include "Setting.h"


CommTask::CommTask(Task *t0, Task *t1){
  IrrigationTask = t0;
  LedTask = t1;
}

void CommTask::init(int period){
  Serial.begin(9600);
  while (!Serial.available());
  MsgService.init();
  MsgServiceBT.init();
  Serial.println("ready to go.");
  Task::init(period);
  state = CHECK_NEW_MESSAGE;
}

/**
    FORMAT:
    [DEVICE]_[PIN]_[servo_speed]
    DEVICE:
      LEDAUTO,
      LEDB,
      LEDF,
      IRRI
    PIN:
      Only 1, 2 or 3 (=for fading both)
    servo_speed:
      Light intensity for fading -> <servo_speed>
      Intensity of irrigation or activation/deactivation -> ON/OFF/<servo_speed>
  */
  void CommTask::tick(){
    switch (state)
    {
      case CHECK_NEW_MESSAGE:
        if (MsgService.isMsgAvailable()) {
          this->msg = MsgService.receiveMsg();
          this->state = EVALUATE_MESSAGE;
          MsgService.sendMsg("S Ricevuto");
        }
        if (MsgServiceBT.isMsgAvailable()) {
          this->msg = MsgServiceBT.receiveMsg();
          this->state = EVALUATE_MESSAGE;
          MsgServiceBT.sendMsg("BT Ricevuto");
        }
        break;

      case EVALUATE_MESSAGE:

        Serial.print("entro evaluate: ");

        msg->getContent().toCharArray(buf, 50);

        Serial.println(buf);
        device = String(strtok(buf,"_"));

        if(device == "ERROR") {
          MsgServiceBT.sendMsg("ERROR");
        } else if(device == "LEDAUTO") {//LEDAUTO_<fade_value> because in auto mode all led must turn on/off
          led_type = device;

          //get the fade value that is setted for the fade leds but also set on the blink leds if is greater than zero
          fade_value = String(strtok(NULL, "_")).toInt();
          LedTask->setActive(true);
        
        } else if(device == "LEDB"){ // led blink manual command
          led_type = device;
          
          String pin = String(strtok(NULL, "_"));
          if(pin == "1")
          {
            Serial.println("Led 1");
            led_id = 1;
          }else if(pin == "2"){
            Serial.println("Led 2");
            led_id = 2;
          } else {//the two blink must coordinate
            led_id = 3;
          }
          //The app send a value equal to 1 just to turn on the led 
          fade_value = String(strtok(NULL,"_")).toInt();//also this wont affect the fading one
          Serial.print("ODIOPORCOH - "); Serial.println(fade_value);
          LedTask->setActive(true);

          
        } else if(device == "LEDF"){//fading led manual command
          led_type=device;
          MsgServiceBT.sendMsg("Blink of an eye");
          Serial.println("Blink of an eye");
          String pin = String(strtok(NULL, "_"));

          if(pin =='1')
          {
            Serial.println("Led 1");
            led_id = 1;
          }else if(pin =='2'){
            Serial.println("Led 2");
            led_id = 2;
          } else {//the two fade must coordinate
            led_id = 3;
          }
          fade_value = String(strtok(buf,"_")).toInt();
          LedTask->setActive(true);
        } else if(device == "IRRI"){

          Serial.println("servo");
          String val = String(strtok(NULL, "_"));
          if(val=="ON"){
            Serial.println("on");
            IrrigationTask->setActive(true);
          }else if(val=="OFF"){
            IrrigationTask->setActive(false);
          }else{
            servo_speed=val.toInt();
            IrrigationTask->init(1000-(val.toInt()*50));
          }
        }

        device = "";
        delete msg;
        this->state = CHECK_NEW_MESSAGE;
        break;

        default:
          Serial.println("DIFOULT");
          break;
    }
  }
