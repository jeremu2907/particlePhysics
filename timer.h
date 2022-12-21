#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <chrono>

using namespace std::chrono;

class timer{
    private:
        unsigned long startTime;
        // const int CLOCK_RATE = 60;
    
    public:
        unsigned long getCurrentTime(){
            return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        }
        void start(){
            startTime = getCurrentTime();
        }

        //In milliseconds
        double elapsedTime(){
            return (getCurrentTime() - startTime);
        }

        bool isTimeOut(double seconds){
            return seconds < (elapsedTime() / 1000);
        }
};

#endif