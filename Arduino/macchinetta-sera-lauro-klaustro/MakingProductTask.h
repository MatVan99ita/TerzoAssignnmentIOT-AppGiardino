#ifndef __SELECTPRODUCTTASK__
#define __SELECTPRODUCTTASK__

#include "Task.h"
#include "Display.h"
#include "ServoMotor.h"
#include "Sonar.h"
#include "Pir.h"
#include "Temperature.h"

class MakingProductTask: public Task {
    enum {WELCOME, READY, GO_SLEEP, WAIT, SELECT, MAKING_PRODUCT, FINISHED_PRODUCT, RETIRED_PRODUCT, ASSISTANCE} states;
    String beverage[3] = {"Coffee", "Tea", "Chocolate"};
    String message;
    Display* displaylcd;
    long tickCounter;
    int period;
    ServoMotor* servo;
    Sonar* sonar;
    int servoCounter;
    Pir* pir;
    Temperature* temperature;

    public:
        MakingProductTask();
        void init(int period);
        void tick();
        };
#endif
