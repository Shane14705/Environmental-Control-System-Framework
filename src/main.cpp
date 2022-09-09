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

//TODO: Eventually use a queue set to block on notifs and semaphores for different objects to allow user control of different peripherals

QueueHandle_t SENSOR_QUEUE;
TimerHandle_t TIMER_HANDLES[NUMINPUTS];

//When created, the "Timer ID" should be a pointer to the timer's DHT sensor object

//TODO: FIX PROBLEM WHERE BOTH DHT SENSORS ARE GONNA OVERWRITE EACH OTHER'S READINGS IN THE QUEUE
void vDHTTimerCallback(TimerHandle_t xTimer) {
  DHT* sensor = (DHT*) pvTimerGetTimerID(xTimer);

  //update the queue with the latest reading
  xQueueOverwrite(
    SENSOR_QUEUE,
    &DHT_READING(sensor->readTemperature(),
  sensor->readHumidity())
  );

}

//control task is higher priority than display task, it will block to peek at notification queue. once it has peeked itll run, then pass it down to display task
//which will unblock and remove it from queue, and update all of the representations
void vCONTROLTASK() {
  for (;;)

}

//Depending on what work is required to keep display up, we might consider making a queue for all changes which would require the display to update
//then we could simply have the display function block on that queue so it only runs when a change is required
void vDISPLAYTASK() {
  //Needs to update display as well as any remote representations (i.e: server)
  for (;;) {
    
    xQueue(SENSOR_QUEUE)
  }
}

Adafruit_SSD1306 OLED1(-1);

std::vector<std::unique_ptr<DHT>> SENSOR_ARRAY;


void setup() {
  //setup our input pins
  int INPUTPINS[NUMINPUTS] = {34, 35};
  setupInputs(INPUTPINS, NUMINPUTS);

  SENSOR_QUEUE = xQueueCreate(1, sizeof(DHT_READING));

  for (int i = 0; i < NUMINPUTS; i++) {
    //delete ptr is just the same as ptr->~Type();
    //very different call from ~ptr();
    std::unique_ptr<DHT> currentsensor = std::unique_ptr<DHT>(new DHT(INPUTPINS[i], DHT22));
    
    currentsensor->begin();

    //as long as the copy constructor for DHT is written correctly, we should be fine 
    SENSOR_ARRAY.push_back(currentsensor);
    TIMER_HANDLES[i] = xTimerCreate(
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