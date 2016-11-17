
#include "Wire.h"
#include <RTClib.h>

RTC_DS1307 rtc;


#define DEBUG 1
#define LED   13

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

void loop() {

  blinkLED(5);
  
}

boolean toggle = 0;
void blinkLED(unsigned char seconds)
{
  unsigned char time;

  toggle = toggle ^ 1;

  time = getSecond() + seconds;
  
  if(time > 60)
  {
    time = time - 60;
  }
  Serial.print(time, DEC);

  while(getSecond() != time)
  {
    
  }

  digitalWrite(LED, toggle);
}

//int getYear()
//{
//  return(now.year());
//}
//
//unsigned char getMonth()
//{
//  return(now.month());
//}
//
//unsigned char getDay()
//{
//  return(now.day());
//}
//
//unsigned char getDayOfWeek()
//{
//  return(now.dayOfTheWeek());
//}
//
//unsigned char getHour()
//{
//  return(now.hour());
//}
//
//unsigned char getMinute()
//{
//  return(now.minute());
//}

unsigned char getSecond()
{
  DateTime now = rtc.now();
  return(now.second());
}
