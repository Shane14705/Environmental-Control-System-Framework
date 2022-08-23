#include <utils.h>
#include <perceiver.h>
#include <vector>
#include <DHT.h>
#define NUMINPUTS 2



void setup() {
  //setup our input pins
  int INPUTPINS[NUMINPUTS] = {34, 35};
  setupInputs(INPUTPINS, NUMINPUTS);
  for (int i = 0; i < NUMINPUTS; i++) {
    //sensors.insert(sensors.cbegin(), );
    
  }
  //Perceiver perception(sensors);

}

void loop() {
  // put your main code here, to run repeatedly:
}