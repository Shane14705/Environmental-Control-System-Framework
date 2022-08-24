#include <Arduino.h>

typedef struct DHTREADING {
    DHTREADING(float tempIN, float humidityIN) : temp(tempIN), humidity(humidityIN) {};
    float temp;
    float humidity;
} DHTREADING;


void setupInputs(int pins[], int numPins);
void setupOutputs(int pins[], int numPins);

// template<typename SENSOR_T>
// struct Sensor {
//     SENSOR_T sensor;
//     uint32_t delay;
//     Sensor(SENSOR_T sensorObject, uint32_t pollDelayMS) : sensor(sensorObject), delay(pollDelayMS), id(_idgen++);

//     private:
//     //Only up to 255 sensors rn
//     static uint8_t _idgen = 0;
//     uint8_t id;
// };