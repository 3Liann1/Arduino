#include <Arduino.h>

// C++ code
#define ServoPin 13
#define LDR1 25
#define LDR2 26
#define LDR3 27
#define WINLEN 5

#include <ESP32Servo.h>
Servo myservo;
int servo_angle=0;
float a,b,c;
float servo_arr[WINLEN];
int servo_count=0;
float servo_sum=90*WINLEN;
void setup()
{
  Serial.begin(9600);
  myservo.attach(ServoPin);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  float a=analogRead(LDR1)/200.0+0.1;
  float b=analogRead(LDR2)+0.5;
  float c=analogRead(LDR3)/200.0+0.1;
  float val1=(90*c-90*a);
  float val2=a+b+c;
  float val3;
  if (val2==0) val2=1;
  val3=val1/val2;
  servo_sum=servo_sum-servo_arr[servo_count]+val3;
  servo_arr[servo_count]=val3;
  servo_count=(servo_count+1)%WINLEN;
  servo_angle=servo_sum/WINLEN;

  Serial.println(servo_angle);
  myservo.write(servo_angle);
  
  delay(100);
}