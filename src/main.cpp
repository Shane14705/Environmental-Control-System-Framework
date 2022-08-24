#include <utils.h>
//#include <perceiver.h>
#include <ctype.h>
#include <vector>
#include <memory>
#include <DHT.h>
#include <Wire.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define NUMINPUTS 2
#define NUMOUTPUTS 4


QueueHandle_t SENSOR_QUEUE;

//When created, the "Timer ID" should be a pointer to the timer's DHT sensor object

//TODO: FIX PROBLEM WHERE BOTH DHT SENSORS ARE GONNA OVERWRITE EACH OTHER'S READINGS IN THE QUEUE
void vDHTTimerCallback(TimerHandle_t xTimer) {
  DHT* sensor = (DHT*) pvTimerGetTimerID(xTimer);

  //update the queue with the latest reading
  xQueueOverwrite(
    SENSOR_QUEUE,
    &DHTREADING(sensor->readTemperature(),
  sensor->readHumidity())
  );

}

void vCONTROLTASK() {

};

void vDISPLAYTASK() {
  for (;;) {
    
    xQueuePeek(SENSOR_QUEUE)
  }
}

Adafruit_SSD1306 OLED1(-1);

std::vector<std::unique_ptr<DHT>> SENSOR_ARRAY;


void setup() {
  //setup our input pins
  int INPUTPINS[NUMINPUTS] = {34, 35};
  setupInputs(INPUTPINS, NUMINPUTS);

  

  for (int i = 0; i < NUMINPUTS; i++) {
    //delete ptr is just the same as ptr->~Type();
    //very different call from ~ptr();
    std::unique_ptr<DHT> currentsensor = std::unique_ptr<DHT>(new DHT(INPUTPINS[i], DHT22));
    SENSOR_QUEUE = xQueueCreate(1, sizeof(DHTREADING));
    currentsensor->begin();

    //as long as the copy constructor for DHT is written correctly, we should be fine 
    SENSOR_ARRAY.push_back(currentsensor);
    xTimerCreate(
      "DHT TIMER",
      (5000/portTICK_RATE_MS),
      pdTRUE,
      currentsensor.get(),
      &vDHTTimerCallback
    );
    
  }


  //Perceiver perception(sensors);

}

void loop() {
  // put your main code here, to run repeatedly:
}