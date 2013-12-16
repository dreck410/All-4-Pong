#include "Timer.h"

Timer* Timer::instance=NULL;

Timer::Timer(){runTimer = new QTimer;}

Timer* Timer::getInstance() {
    if (instance == NULL) {
        instance = new Timer();
    }
    return instance;
}
