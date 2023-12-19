#ifndef __CHANGESUGARLEVELTASK__
#define __CHANGESUGARLEVELTASK__

#include "Task.h"
#include "Potentiometer.h"

class ChangeSugarLevelTask: public Task {
    int potPin;
    Potentiometer* potentiometer;
    enum {UPDATE_LEVEL} states;

    public:
        ChangeSugarLevelTask(int PotPin);
        void init(int period);
        void tick();
};
#endif
