#include <utils.h>
//#include <perceiver.h>
#include <ctype.h>
#include <vector>
#include <memory>
#include <DHT.h>
#include <Wire.h>
#include<Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdatomic.h>
#include <math.h>
#include <stdlib.h>

#include "config.h"

Adafruit_SSD1306 DISP1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 DISP2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//TODO: Eventually use a queue set to block on notifs and semaphores for different objects to allow user control of different peripherals

QueueHandle_t SENSOR_QUEUE;
TimerHandle_t TIMER_HANDLES[NUMINPUTS];
SemaphoreHandle_t UPDATE_DISPLAY;

int oldPotVal = 0;

atomic_uint_fast16_t currentTemp = nullptr;
atomic_uint_fast16_t currentHumidity = nullptr;

uint16_t oldTemp = currentTemp;
uint16_t oldHumid = currentHumidity;

//When created, the "Timer ID" should be a pointer to the timer's DHT sensor object

//TODO: FIX PROBLEM WHERE BOTH DHT SENSORS ARE GONNA OVERWRITE EACH OTHER'S READINGS IN THE QUEUE
void vDHTTimerCallback(TimerHandle_t xTimer) {
  DHT* sensor = (DHT*) pvTimerGetTimerID(xTimer);


  //Update status
  currentTemp = (int) roundf(sensor->readTemperature(true));
  currentHumidity = (int) roundf(sensor->readHumidity());

  if (currentTemp != oldTemp || currentHumidity != oldHumid) {
    //Trigger a display update
    xSemaphoreGive(UPDATE_DISPLAY);    
  }

}

//should have higher priority than display task
void vINTERFACETASK() {
  for (;;) {
    if (analogRead(34) != oldPotVal) {
      analogRead(34) == oldPotVal;
      //VULNERABILITY: OVERFLOW -- think i can handle it by making sure i take abs of millis - calltime, since it prevents negatives
      int calltime = millis();
      DISP1.clearDisplay();
      while (abs((long)(millis() - calltime)) < POTDISPDELAY) {
        if (analogRead(34) != oldPotVal) {
          analogRead(34) == oldPotVal;
          calltime == millis();
        }
        DISP1.setCursor(64, 32);
        DISP1.setTextSize(5);
        DISP1.setTextColor(WHITE, BLACK);
        DISP1.printf("%d", analogRead(34));
      }
      

      //After time out of while loop, refresh screen
      xSemaphoreGive(UPDATE_DISPLAY);

    }

  }

}

//Responsible for turning peripherals on and off. Can receive notifications through queue either from user control or from the sensor reading task
void vCONTROLTASK() {
  for (;;) {
  }
}

//Depending on what work is required to keep display up, we might consider making a queue for all changes which would require the display to update
//then we could simply have the display function block on that queue so it only runs when a change is required

//Handles refreshing the display as well as updating any websockets. To request a display refresh, a task must "give" the UPDATE_DISPLAY semaphore
void vDISPLAYTASK() {
  //Needs to update display as well as any remote representations (i.e: server)

  //Dont have to worry about it still being null here, since if it was null at the start then oldtemp/humid would be null and current would be same as null.
  
  for (;;) {

    //block until we are told to update again -- blocking on websockets may cause issues as the semaphore may be given again by another task while the websocket is blocking, causing the display loop to run again
    xSemaphoreTake(UPDATE_DISPLAY, portMAX_DELAY);
    DISP1.setTextSize(1);
    if (currentTemp != oldTemp) {
      //write new temp to display
      DISP1.setCursor(64, 32);
      DISP1.setTextColor(WHITE, BLACK);
      DISP1.printf("Temp: %d", currentTemp);

      oldTemp = currentTemp;
      //DISP1.getTextBounds() use later to determine offsets from center
    }
    if(currentHumidity != oldHumid) {
      //write new humidity to display
      DISP1.setCursor(64, 42);
      DISP1.setTextColor(WHITE, BLACK);
      DISP1.printf("Humidity: %d", currentHumidity);

      oldHumid = currentHumidity;
    }
    //after this, run loop again where we will block since we already took the semaphore (until someone else "gives" it again, triggering another update)
  }
}



std::vector<std::unique_ptr<DHT>> SENSOR_ARRAY;



void setup() {
  //setup our input pins
  int INPUTPINS[NUMINPUTS] = {34, 35, 36, 39};
  setupInputs(INPUTPINS, NUMINPUTS);

  int OUTPUTPINS[NUMOUTPUTS] = {16, 17, 18, 19};
  setupOutputs(OUTPUTPINS, NUMOUTPUTS);

  SENSOR_QUEUE = xQueueCreate(1, sizeof(DHT_READING));
  UPDATE_DISPLAY = xSemaphoreCreateBinary();

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