#include <Arduino.h>

#define BtnStart 13
#define BtnJump 12
#define LEDDIn 27
#define LEDCS 26
#define LEDCLK 25
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <cstdlib>

using namespace std;

void game_loop();

bool in_game=false;
bool SBtnPreState=HIGH;
bool JBtnPreState=HIGH;

MD_MAX72XX Display = MD_MAX72XX(MD_MAX72XX::PAROLA_HW,LEDDIn,LEDCLK,LEDCS,1);

int Sprite[6][2]={{0,6},{1,6},{1,5},{1,4},{1,3},{2,3}};
int Feet[2][2]={{1,7},{0,7}};
int FeetInd=0;
int JTrig=0;
int JPos=0;
int ObsPos=8;
int Speed=100;
int SCount=0;
int score=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Press top push button to start/stop!");
  Serial.println("Press bottom push button to jump!");
  pinMode(BtnStart, INPUT_PULLUP);
  pinMode(BtnJump, INPUT_PULLUP);
  Display.begin();
  Display.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  
  Display.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  int SBtn=digitalRead(BtnStart);
  if (in_game==false){
    if ((SBtn==LOW)&(SBtnPreState==HIGH)){
      Serial.println("Start");
      delay(100);
      in_game=true;
      JBtnPreState=HIGH;
      Speed=100;
      JTrig=0;
      JPos=0;
      ObsPos=8;
      SCount=0;
      score=0;
      Display.clear();
    }
  }
  else{
    game_loop();
    if ((SBtn==LOW)&&(SBtnPreState==HIGH)){
      Serial.println("Stop");
      delay(100);
      in_game=false;
      JBtnPreState=HIGH;
      Speed=100;
      Display.clear();
      Display.setPoint(0,0,true);
      Display.update();
    }
  }
  SBtnPreState=SBtn;
  delay(Speed); // this speeds up the simulation
}

void game_loop(){
  Display.clear();
  int JBtn=digitalRead(BtnJump);
  if ((JBtn==LOW)&&(JBtnPreState==HIGH)&&(JTrig==0)){
    Serial.println("Jump");
    JTrig=1;
  }
  if (JTrig==1){
    if (JPos<3) JPos+=1;
    if (JPos==3) JTrig=2;
  }
  else if (JTrig==2){
    if (JPos>0) JPos-=1;
    if (JPos==0) JTrig=0;
  }
  //Drawing
  for (int i=0;i<6;i++) Display.setPoint(Sprite[i][0],Sprite[i][1]-JPos,true);
  Display.setPoint(Feet[FeetInd][0],Sprite[FeetInd][1]-JPos+1,true);
  if (ObsPos!=8) Display.setPoint(ObsPos,7,true);
  //Collision
  if ((ObsPos==1)&&(JPos==0)){
    Serial.println("Game Over");
    Serial.print("Score ");
    Serial.println(score);
    in_game=false;
    Display.clear();
    return;
  }

  FeetInd=(FeetInd+1)%2;
  JBtnPreState=JBtn;
  SCount++;
  if (SCount==10){
    SCount=0;
    Speed--;
  }
  score+=100-Speed;
  float rand_out=((float)rand())/RAND_MAX;
  Serial.println(rand_out);
  if ((rand_out<0.1)&&(ObsPos==8)) ObsPos=7;
  else if ((ObsPos>0)&&(ObsPos!=8)) ObsPos--;
  else ObsPos=8;
  Display.update();
}
