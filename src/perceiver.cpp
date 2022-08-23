#include <perceiver.h>
#include <utils.h>

//Sensor delay in milliseconds. Eventually this will be passed in as part of a struct that allows for user defined sensors.
#define SENSOR_DELAYMS 5000

//This task should simply poll the given sensor, and send a task notification the controller task with the new readings
void xDHTTimerCallback(TimerHandle_t handle) {

};

Perceiver::Perceiver(std::vector<DHT> dhtContainer) {
    //Constructor: what is this class's invariant? What should always be true about it?


    for (int i = 0; i < dhtContainer.size(); i++) {
        dhtContainer[i].begin();
        TimerHandles.insert(TimerHandles.cbegin(), xTimerCreate(NULL, (SENSOR_DELAYMS / portTICK_RATE_MS), pdTRUE, NULL, &xDHTTimerCallback));
        
    }

};