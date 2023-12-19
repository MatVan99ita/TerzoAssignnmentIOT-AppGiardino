#include "Display.h"
#include "Arduino.h"
#include "Config.h"

Display::Display(){
    lcd.init();
    lcd.backlight();
}

void Display::addString(String displayedString, int row){
    /**pulisce l'output del monitor*/
    //lcd.clear();
    /** posiziona il cursore sulla prima riga e colonna */
    lcd.setCursor(0, row);
    /** mostra una stringa sul monitor */
    lcd.print(displayedString);
}

void Display::showString(String displayedString){

    if(isSugarSettable){
      displayedString = String(displayedString + "              sugar:" + sugarLevel );
    }
    
    /**pulisce l'output del monitor*/
    lcd.clear();
    /** posiziona il cursore sulla prima riga e colonna */
    lcd.setCursor(0, 1);
    /** mostra una stringa sul monitor */
    lcd.print(displayedString);

}
