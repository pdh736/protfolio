/*
 WiFiEsp example: WebClient

 This sketch connects to google website using an ESP8266 module to
 perform a simple web search.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/
/*
  # the sensor value description
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water
*/
#include "WiFiEsp.h"
#include <Servo.h>
#include <LiquidCrystal.h>
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
#include <MsTimer2.h>
SoftwareSerial Serial1(6, 7); // RX, TX
#endif
#define motor 9         //servo motor pin
void setTimer(char *pArray[]);

char ssid[] = "embsystem";            // your network SSID (name)
char pass[] = "embsystem0";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.1.167";
#define PORT 5000

#define ARR_CNT 10
#define CMD_SIZE 50
#define INTERVALTIME 5000
// Initialize the Ethernet client object
WiFiEspClient client;
LiquidCrystal lcd(12,13,5,4,3,2); //RS,E,DB4,DB5,DB,DB7
char sendBuf[CMD_SIZE];
char recvBuf[CMD_SIZE];
unsigned long timerTick=0;

int sensor_read=0;  //flag sensor read

Servo myservo;

void setup()
{ 

  pinMode(A0,INPUT);//moisture sensor
  
//--------------wifi setting-----------------
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  // you're connected now, so print out the data
//  Serial.println("You're connected to the network");
  printWifiStatus();
  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, PORT)) {
    Serial.println("Connected to server");
    // Make a HTTP request
  }
//-----------------------------------------------

 //-----timer setting-------------------------
  MsTimer2::set(1000,timerIsr);   //1000ms timer
  MsTimer2::start();  
//------------lcd setting---------------------
  lcd.begin(16,2);//setup lcd columns and rows
  lcd.clear();
  lcd.print("Hello");
}

void loop()
{ 
  int moisture;
  //char humid[5];
  char msg[15];
  int val;
  float temperature;
  // if there are incoming bytes available
  // from the server, read them and print them
  
  while (client.available()) {
    socketEvent();
  }
  while (Serial.available()) {
     int len =Serial.readBytesUntil('/',recvBuf,CMD_SIZE);
//     int len =client.readBytesUntil(0x0a,recvBuf,CMD_SIZE); 
     client.write(recvBuf,len);
  }

  // if the server's disconnected, stop the client
  if(!(timerTick % 5)) {
    if (!client.connected()) {
      Serial.println("Disconnecting from server...");
      client.stop();
      if (client.connect(server, PORT)) {
        Serial.println("Connected to server");
      }
    }
   }
//sonsor read  &  transmit vlaues to server 
  if(sensor_read){
     if(!(timerTick % 7)) {
       moisture=analogRead(A0);
       val=analogRead(A1);
       temperature=(5.0*val*100)/1024;
       lcd.clear();
       lcd.print(moisture);
       lcd.setCursor(0,1);
       lcd.print(temperature);       
       sprintf(msg,"%d:%s",moisture,String(temperature).c_str());
       //String(temperature).c_str() float -> string
      Serial.println(String(temperature).c_str());      
       client.write(msg,strlen(msg));
       sensor_read=0;
     }
  }
}
void socketEvent()
{
   int i;
  // char *pToken;
  // char *pArray[ARR_CNT]={0};
   int len;
   
   /*  
   //-----------passing-------------
   pToken = strtok(recvBuf,"[#]");
   i = 0;
   while(pToken != NULL)
   {
      pArray[i] =  pToken;
      if(++i >= ARR_CNT)
          break;
      pToken = strtok(NULL,"[#]");
   }
   //-------------------------------
   */
   
   memset(recvBuf,0,CMD_SIZE);
   len =client.readBytesUntil('/',recvBuf,CMD_SIZE); 
//   len =client.readBytesUntil(0x0a,recvBuf,CMD_SIZE); 
   recvBuf[len]='\0';
   if(strcmp(recvBuf,"low\n")==0){
      Serial.write(recvBuf,len);
      myservo.attach(motor);
      if(myservo.attached())Serial.println("1");
      myservo.write(180);
      delay(1000);
      myservo.write(0);
      delay(200);
      myservo.detach();        
    }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void timerIsr()
{
    timerTick++;
    sensor_read=1;
}


