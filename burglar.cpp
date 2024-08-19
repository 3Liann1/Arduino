#include <Arduino.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif


BluetoothSerial SerialBT;

const int trigpin = 12;
const int echopin = 14;
const int buzzer = 15;
const int LED = 2;

int count=0;
float duration;
float distanceCm;
bool triggered=false;
String dat="danger";

#define SOUND_SPEED 0.034
void Message(int);
void Alarm();

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(LED,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  
  duration = pulseIn(echopin, HIGH);
  distanceCm = (duration * SOUND_SPEED)/2;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  if (distanceCm>10){
    Alarm();
    if (!triggered) Message(count);  
  }
  else {
    triggered=false;
    delay(50);
  }
}
void Alarm(){
  digitalWrite(LED,HIGH);
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
    digitalWrite(LED,LOW);
    delay(50); 
  }
void Message(int cnt){
  triggered=true;
  SerialBT.println(dat);
}