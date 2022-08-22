#include <perceiver.h>
#include <utils.h>

//Sensor delay in milliseconds. Eventually this will be passed in as part of a struct that allows for user defined sensors.
#define SENSOR_DELAYMS 5000


Perceiver::Perceiver(std::vector<DHT> dhtContainer) {
    //Constructor: what is this class's invariant? What should always be true about it?


    for (DHT sensor : dhtContainer) {
        //xTimerCreate(NULL, (SENSOR_DELAYMS / portTICK_RATE_MS), pdTRUE, )
        //Need to figure out how the timer task should share its polled values with the other tasks.
    }

};