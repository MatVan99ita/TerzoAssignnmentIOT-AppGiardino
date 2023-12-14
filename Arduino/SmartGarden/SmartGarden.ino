
void setup() 
{
  Serial.begin(9600); 
  MyBlue.begin(9600);  //Baud Rate for AT-command Mode.
  
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() 
{ 

}
