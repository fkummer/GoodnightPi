// Written by Nick Gammon
// April 2011
#include <SPI.h>

volatile unsigned char SPI_buff[100];
volatile byte cnt = 0;
volatile byte pos = 0;
volatile byte resp = 0;
volatile byte incoming = 0;
volatile byte outgoing = 0;
unsigned char resp_buff[11];
volatile unsigned char recv_packet[11];
volatile byte resp_ind = 0;

#define PACKET_LEN 11

void setup (void)
{
  Serial.begin(115200);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}  // end of setup


// SPI interrupt routine
//Capture what is coming in. 
ISR (SPI_STC_vect)
{
  //Serial.println("caw");
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
  
}  // end of interrupt service routine (ISR) SPI_STC_vect

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
      buff[1] = 'w';
      buff[2] = 'a';
      buff[3] = 'k';
      buff[4] = 'e';
      
      //1 byte for if interrupt 0 woke it or not
      buff[5] = 'm';
      
      //1 byte for if interrupt 1 woke it or not
      buff[6] = 'e';
      
      //4 empty bytes
      buff[7] = 'u';
      buff[8] = 'p';
      buff[9] = '!';
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

//Specifically for populating empty payloads, since its so common.
void fillEmpty(unsigned char * buff){
  byte buff_ind;
  for(buff_ind = 1; buff_ind < PACKET_LEN; buff_ind++){
    buff[buff_ind] = 'a';
  }
}

void loop (void)
{
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
      //Serial.println(ind);
      Serial.write(SPI_buff[ind]);
      ind++;
    }
    Serial.print("\n");
    cnt = 0;

    if(outgoing){

      resp_buff[0] = getOpCode(recv_packet[0]);
      fillPayload(resp_buff[0], resp_buff); 
      SPDR = resp_buff[0];
      resp_ind = 1;
      outgoing = 0;
    }
  }
}
