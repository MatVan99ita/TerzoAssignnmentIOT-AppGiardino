TaskSystem_On(){
  PIN1=on;
  PIN2=on;
  PIN3=on;
  }

TaskTemp_Control(){
  Temperatura=1; //"readAnalogPin"
  }

Water(){
  Servo=1;
  delay(1000);
  Servo=0;
  }

TaskSystem_Off(){
    PIN1=off;
    PIN2=off;
    PIN3=off;
  }

Autoblock(){
  if Storm=1{
     Request_enable();
      }
 
  }

Manual(){
  
  }

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
