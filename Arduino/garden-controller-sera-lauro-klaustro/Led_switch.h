#ifndef __LED_SWITCH__
#define __LED_SWITCH__

#include "Led.h"

class Led_switch: public Led{
    public:
    Led_switch(int pin);
    void switchOff();
    void switchOn();
    bool change();

    private:
    int pin;
};

#endif
