#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define bt_rx 3
#define bt_tx 4

SoftwareSerial bt(bt_rx,bt_tx);
LiquidCrystal lcd(7,6,8,9,10,11); //RS,E,DB4,DB5,DB,DB7

bool mode=false;  //manual:0   auto: 1 

void lcd_set(const char mode[]); 
void isr0(){
  delay(300);  //debouncing
  if(digitalRead(2)==1){
    mode=!mode;
  }
}

void setup() { 
  bt.begin(9600);
  lcd.begin(16,2);
  pinMode(A0,INPUT);
  pinMode(12,INPUT);//sw 1 open
  pinMode(13,INPUT);//sw 2 close
  attachInterrupt(0,isr0,RISING);//inturrupt0(pin2) , 함수 ,모드
}

void loop() {
     if(mode){//auto
      lcd_set("auto"); 
      if(analogRead(A0)<650)bt.write('o');
      else if (analogRead(A0)>=650)bt.write('c');
      delay(1000);
    }
    else{//manual
      lcd_set("manual");
      if(digitalRead(12)==1)bt.write('o');
      if(digitalRead(13)==1)bt.write('c');
      delay(300);
    }  
}

void lcd_set(const char mode[]){
      lcd.clear();
      lcd.print("MODE : ");
      lcd.setCursor(7,0);
      lcd.print(mode);    
}


