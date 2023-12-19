#ifndef __DISPLAY__
#define __DISPLAY__
#include "Task.h"
#include <LiquidCrystal_I2C.h>

class Display {

  public:
    Display();
    void addString(String displayedString, int row);    
    void showString(String displayedString);

  private:
    /** indirizzo I2C , numero righe, numero colonne */
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
  };

#endif
