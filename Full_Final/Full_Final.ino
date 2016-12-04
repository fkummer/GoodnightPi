
#include "Wire.h"
#include <RTClib.h>
#include <SPI.h>

//Variables for I2C
RTC_DS1307 rtc;
long wakeTime;
char timeSet = 0;
char ledState = 0;

//Variables for SPI
volatile unsigned char SPI_buff[100];
volatile byte cnt = 0;
volatile byte pos = 0;
volatile byte resp = 0;
volatile byte incoming = 0;
volatile byte outgoing = 0;
unsigned char resp_buff[11];
volatile unsigned char recv_packet[11];
volatile byte resp_ind = 0;

//Is Pi alive
boolean alive = 1;

//Do we want to turn off the power when we get the chance?
boolean gotoSleep = 0;

//timeInterval for our next wakeup.
long timeInterval = 0;

#define LED   7
#define PACKET_LEN 11
#define ALIVE 5
#define PWR_CNTRL 4

// SPI interrupt routine
//Capture what is coming in. 
ISR (SPI_STC_vect)
{
  if(pos > 99){
    pos = 0;
  }
  
  //Serial.println(SPDR);
  
  //If we are receiving a packet
  if(incoming){
    cnt++;
    //Serial.println(SPDR);
    SPI_buff[pos] = SPDR;
    pos++;
    if(cnt >= PACKET_LEN){
      incoming = 0;
      outgoing = 1;
    }
  }else{//Look for start byte
    if(SPDR == 0xFE){
      incoming = 1;
      
      outgoing = 0;
      cnt = 0;
    }
  }
  
  
  //The master is asking us for a response and we expect to return one.
  
  //HERES THE PROBLEM: We are overwriting the SPDR so we can't get this 0xFF? Or is this pi sending us Y? Check on scope.
  if(SPDR == 0xFF){
    //Serial.println("AH");
    SPDR = resp_buff[resp_ind];
    resp_ind++;
  }
  
}// end of interrupt service routine (ISR) SPI_STC_vect

byte getOpCode(byte receivedOpCode){
  switch (receivedOpCode){
    case 0x00://Respond to WAKE_REQ with WAKE_RESP
      return 0x01;
      break;
    case 0x02:
      return 0x03;
      break;
    case 0x04:
      return 0x05;
      break;
    case 0x06:
      return 0x07;
      break;
    default: 
      return 0xFF;
      break;
  }
}  

void fillPayload(byte opCode, unsigned char * buff){
  switch (opCode){
    case 0x01: //WAKE_RESP
      //4 bytes for the time interval that woke it
      buff[1] = timeInterval & 0x000000FF;
      buff[2] = timeInterval & 0x0000FF00;
      buff[3] = timeInterval & 0x00FF0000;
      buff[4] = timeInterval & 0xFF000000;
      
      //1 byte for if interrupt 0 woke it or not
      buff[5] = 0;
      
      //1 byte for if interrupt 1 woke it or not
      buff[6] = 0;
      
      //4 empty bytes
      buff[7] = 0;
      buff[8] = 0;
      buff[9] = 0;
      buff[10] = 0x00;
      
      break;
      
    case 0x03: //SLEEP_RESP
      fillEmpty(buff);
      break;
      
    case 0x05: //CONFIG_RESP
      fillEmpty(buff);
      break; 
    
    case 0x07: //PING_RESP
      fillEmpty(buff);
      break;
    
    default:
      fillEmpty(buff);
      break;
  }
 
} 

//Set things as needed.
void processPacket(volatile unsigned char * buff){
  unsigned char opCode = buff[0];
  
  switch(opCode){
    case 0x00://WAKE_REQ
      //We dont need to do anything!
      break;
      
    case 0x02://SLEEP_REQ
      //Time to sleep
      Serial.println("Im Sleepy");
      gotoSleep = 1;
      break;
      
    case 0x04:
      int i;
      //Assemble the time interval!
      //Fetchez la vache!
      timeInterval = 0x00000000;
      for(i = 4; i > 0; i--){
        timeInterval = timeInterval | buff[i];
        if(i > 1){
          timeInterval = timeInterval << 8;
        }
      }
      Serial.println("Configured");
      Serial.println(timeInterval);
      //TODO: Add interrupt stuff
      
      break;
      
    case 0x06:
      //We dont need to do anything!
      break;
      
    default:
      break;
  }
  
}
//Specifically for populating empty payloads, since its so common.
void fillEmpty(unsigned char * buff){
  byte buff_ind;
  for(buff_ind = 1; buff_ind < PACKET_LEN; buff_ind++){
    buff[buff_ind] = 'a';
  }
}

//Pass in the number of seconds until next wake up, calculate 
//the appropriate unix time for to wake up. Set the timer flag
void setWakeupTime(long seconds){
  Serial.println("Set");
  wakeTime = rtc.now().unixtime() + seconds;
  timeSet = 1;
}

char checkWakeup(){
  //Serial.println(timeSet, DEC);
  if(timeSet != 0){
    //Serial.println("in");
    if(rtc.now().unixtime() >= wakeTime){
      //Time to get up!
     timeSet = 0;
     Serial.println("hm");
     return 1;
     
     
    }
  }
  //Serial.println("abafadfadsfa");
  return 0;
}

void handleSPI(){
  //Received a full packet, lets print it
  //We should also get our response ready!
  if(cnt == PACKET_LEN){  
    //Print out the packet
    byte ind;
    
    if(pos < PACKET_LEN){
      ind = 99-(pos+1);
    }else{
      ind = (pos-PACKET_LEN)%100;
    }
    
    for(int i = 0; i < PACKET_LEN; i++){
      if(ind > 99){
        ind = 0;
      }
      recv_packet[i] = SPI_buff[ind];
      ind++;
    }
    cnt = 0;

    if(outgoing){
      processPacket(recv_packet);
      resp_buff[0] = getOpCode(recv_packet[0]);
      fillPayload(resp_buff[0], resp_buff); 
      SPDR = resp_buff[0];
      resp_ind = 1;
      outgoing = 0;
    }
  }
}

void turnOffPower(){
  Serial.println("going down");
  gotoSleep = 0;
  digitalWrite(PWR_CNTRL, LOW);
  return;
}

void setup() {
  Serial.begin(115200);
  
  
  //SPI setup
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // turn on interrupts
  SPCR |= _BV(SPIE);
  
  //I2C Setup
  rtc.begin();

  pinMode(LED, OUTPUT);
  pinMode(PWR_CNTRL, OUTPUT);
  pinMode(ALIVE, INPUT);
  
  delay(5);
}


void loop() {
  alive = digitalRead(ALIVE);
  
  if(gotoSleep){
    if(!alive){
      Serial.println("Beep");
      delay(5000); //Wait to make sure Pi is shutdown
      turnOffPower();
      if(timeInterval != 0){
        Serial.println("Alarming!");
        setWakeupTime(timeInterval);
      }
    }
  }
  
  if(alive){
   //Receive any incoming packets and send a response
   //Serial.println("Live");
   handleSPI();
  }
  
  //Time to wakeup!
  if(checkWakeup() == 1){
     Serial.println("BRING!");
     ledState = ledState ^ 1;
     digitalWrite(LED,ledState);
     digitalWrite(PWR_CNTRL, HIGH);
  }else{
  
  }
}
