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
}

CheckTemp(){
   Temperatura=1; //"readAnalogPin"
   if (Temperatura>Soglia){
      
    }
  
  }



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
   CheckLight();
   CheckTemp();
}
