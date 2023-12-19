#ifndef __POTENTIOMENTER__
#define __POTENTIOMENTER__

class Potentiometer{
    public:
    Potentiometer(int pin);
    int getValue();

    private:
    int pin;
};

#endif