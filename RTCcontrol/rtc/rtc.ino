
#include "Wire.h"
#include <RTClib.h>

RTC_DS1307 rtc;

#define LED   13

long wakeTime;
char timeSet = 0;
char ledState = 0;

void setup() {
  // put your setup code here, to run once:
  unsigned char flag;
  
  Serial.begin(9600);
  Serial.print("Initializing RTC...");
  rtc.begin();

  pinMode(LED, OUTPUT);
  
  flag = rtc.isrunning();
  Serial.print(flag, DEC);
  if(flag == 0)
  {
    Serial.print("\n+5V not connected");
  }
  else
  {
    Serial.print("\nRTC Working");
  }
  
}

boolean toggle = 0;

//Pass in the number of seconds until next wake up, calculate 
//the appropriate unix time for to wake up. Set the timer flag
void setWakeupTime(long seconds){
  wakeTime = rtc.now().unixtime() + seconds;
  timeSet = 1;
}

boolean checkWakeup(){
  if(timeSet){
    Serial.println(rtc.now().unixtime());
    delay(1000);
    if(rtc.now().unixtime() >= wakeTime){
      //Time to get up!
     return true;
    }else{
     return false;
    }
  }
}

void loop() {
     setWakeupTime(5);
     while(!checkWakeup());
     
     ledState = ledState ^ 1;
     digitalWrite(LED,ledState);
  
}
