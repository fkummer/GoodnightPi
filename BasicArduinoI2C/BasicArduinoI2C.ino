#include <Wire.h>

int LED_stat = 0;

void setup(){
  Wire.begin(10);
  Wire.onReceive(receiveEvent);
  pinMode(13, OUTPUT);
}

void loop(){
  delay(100);
}

void receiveEvent(int numBytes){
  LED_stat = ~LED_stat;
  digitalWrite(13, LED_stat);
}
