#ifndef __TEMPERATURECHECKTASK__
#define __TEMPERATURECHECKTASK__

#include "Task.h"
#include "temperature.h"
#include "ServoMotor.h"


class MachineCheckTask: public Task {
    int temperaturePin;
    Temperature* temperature;
    enum {WAITING_FOR_CHECK, SERVO_CHECK, TEMPERATURE_CHECK,GO_ASSISTANCE} states;
    ServoMotor* servo;
    int servoCounter;
    long tickCounter;
    int period;
    int angle;
    int counter;

    public:
        MachineCheckTask(int temperaturePin);
        void init(int period);
        void tick();
        };
#endif
