#include <utils.h>
//#include <perceiver.h>
#include <vector>
#include <memory>
#include <DHT.h>
#define NUMINPUTS 2

void xDHTTimerCallback();

std::vector<std::unique_ptr<DHT>> SENSOR_ARRAY;

void setup() {
  //setup our input pins
  int INPUTPINS[NUMINPUTS] = {34, 35};
  setupInputs(INPUTPINS, NUMINPUTS);

  for (int i = 0; i < NUMINPUTS; i++) {
    //delete ptr is just the same as ptr->~Type();
    //very different call from ~ptr();
    SENSOR_ARRAY.push_back(std::unique_ptr<DHT>(new DHT(INPUTPINS[i], DHT22)));
    
  }

  //Perceiver perception(sensors);

}

void loop() {
  // put your main code here, to run repeatedly:
}