#include <Arduino.h>
#include <queue>
#define HORI 4
#define PUSH 21

using namespace std;

TaskHandle_t task1;
TaskHandle_t task2;


#define QUEUESIZE 10
queue <pair <int,int> > myqueue;


int prevtime=0;
float time_taken;
float accelaration_x,velocity;
bool processing = true;

void IRAM_ATTR estop() {

processing = false;}

void IRAM_ATTR release() {

processing = true;}

void task1code(void* pvparameters) {
  for (;;) {
    pair <int,int> mypair;
    mypair.first = analogRead(HORI);
    mypair.second = millis();

    myqueue.push(mypair);
    delay(100);
    }
  }

void task2code(void* pvparameters) {
  for (;;) {
    if (processing) {
      if (!myqueue.empty()) {
        pair <int,int> mypair=myqueue.front();
        myqueue.pop();
        int current_time = mypair.second;
        time_taken = (current_time - prevtime) / 1000.0;
        accelaration_x = map(mypair.first, 0, 5400, -10, 10);
        //Serial.println(mypair.first);
        velocity += accelaration_x * time_taken;
        prevtime = current_time;
        Serial.println(velocity);
      }
    } else {
      Serial.println("0");}
    delay(100);
    }
  }

void setup() {
  pinMode(PUSH,PULLUP);
  Serial.begin(9600);
  pinMode(18,INPUT);
  pinMode(19,INPUT);
  pinMode(21,INPUT);
  xTaskCreatePinnedToCore(task1code, "TASK1", 10000, NULL, 1, NULL, 1);
  delay(100);
  xTaskCreatePinnedToCore(task2code, "TASK2", 10000, NULL, 0, NULL, 0);
  delay(100);
  Serial.println("starting..");
}

void loop(){}