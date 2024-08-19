#include <MD_MAX72xx.h>
#define CS 15
#define MOSI 27
#define CLK 2
#define VERT 26
#define HORI 25
#define PUSH 13

int vval = 4;
int hval = 4;
int p =24;
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW,MOSI,CLK,CS, 1);
int array[8][8][2];
bool t = true;
int dead = 0;
int currdead = 0;
int score[2];

void dist(int x,int y){
  mx.setPoint(vval,hval,false);
  mx.update();
  for(int i=0;i<8;i++)
  for(int j=0;j<8;j++){
  if(mx.getPoint(i,j)){
  array[i][j][1] = (i-x)*(i-x) + (j-y)*(j-y);
  if(array[i][j][1]<int((esp_random()%98)/p)){
  mx.setPoint(i,j,false);
  array[i][j][0] = 0;
  currdead+=1;}
  mx.update();
  }}}

void setup(){
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  mx.clear();
  Serial.begin(9600);
  Serial.println("lets begin the game!");
  Serial.println("Player 1's turn");
  
  for(int i=0;i<8;i++) for(int j=0;j<8;j++){
  mx.setPoint(i,j,true);
  array[i][j][0]=1;  }
  mx.update();}

void loop(){
  t = !t;
  if(!t) Serial.println("Player 1's turn");
   else Serial.println("Player 2's turn");
  currdead=0;
  delay(1000);
  for(int k = 0;k<20;k++){
  int v = int(analogRead(VERT));
  int h = int(analogRead(HORI));
  if (v>4000 && vval<7) vval += 1;
  if (h>=4000 && hval<7) hval += 1;
  if (v<=100 && vval>0) vval -= 1;
  if (h<=100 && hval>0) hval -= 1;
  bool state = array[vval][hval][0];
  mx.setPoint(vval,hval,!state);
  mx.update();
  delay(100);
  mx.setPoint(vval,hval,state);
  mx.update();
  delay(100);
  }
  dist(vval,hval);
  score[int(t)]+=dead*currdead;
  dead+=currdead;
  Serial.println("scores: ");
  Serial.print(score[0]);
  Serial.print("and");
  Serial.println(score[1]);
  if(dead>=64){
  if(score[0]>score[1])
  Serial.println("Player 1 won the match!");
  else Serial.println("Player 2 won the match!");
  while (1);}}
