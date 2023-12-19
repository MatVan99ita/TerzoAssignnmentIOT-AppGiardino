#ifndef __LED_FADE__
#define __LED_FADE__

#include "Led.h"

class Led_fade: public Led{
    public:
    Led_fade(int pin);
    
    void fade(int v);

    private:
    int pin;
};

#endif
