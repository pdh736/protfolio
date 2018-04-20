#include <SoftwareSerial.h>

#define IN1 10      //stepmotor pin
#define IN2 11      //stepmotor pin
#define IN3 12      //stepmotor pin
#define IN4 13      //stepmotor pin
#define bt_rx 8
#define bt_tx 9

SoftwareSerial bt(bt_rx,bt_tx);

void clockwise();
void c_clockwise();
void set_state(int i);
void open(int num);
void close(int num);

char val;     //command
int cnt=0;
char blind='c';//blind state
int state[8] = {0b1000, 0b1100, 0b0100, 0b0110, 0b0010, 0b0011, 0b0001, 0b1001};
void setup() {
  pinMode(IN1,1); //stepmotor pin - output
  pinMode(IN2,1); //stepmotor pin - output
  pinMode(IN3,1); //stepmotor pin - output
  pinMode(IN4,1); //stepmotor pin - output
  bt.begin(9600);
}

void loop() {
  if(bt.available()){
      val=bt.read(); //read command
      if(blind=='o'){  
          if(val=='c'){
            cnt=0;
            close(1);
            blind='c';
          }
      }
      if(blind=='c'){
          if(val=='o'){
            cnt=0;
            open(1);
            blind='o';
          }
      }
  }
}

void set_state(int i){
  digitalWrite(IN1,bitRead(state[i],0)); //state arry 0 bit read
  digitalWrite(IN2,bitRead(state[i],1));
  digitalWrite(IN3,bitRead(state[i],2));
  digitalWrite(IN4,bitRead(state[i],3));
}
void clockwise(){
  for(int i=7;i>=0;i--){
  set_state(i);
  delay(1);
  }   
}
void c_clockwise(){
  for(int i=0;i<8;i++){
  set_state(i);
  delay(1);
  }   
}
void open(int num){
  while(cnt<(512*num)){//1 Stride Angle 5.625/64 
    clockwise();      //(5.625/64)8 * 512 -> 360
    cnt++;
  }
}
void close(int num){
  while(cnt<(512*num)){
    c_clockwise();
    cnt++;
  }
}
