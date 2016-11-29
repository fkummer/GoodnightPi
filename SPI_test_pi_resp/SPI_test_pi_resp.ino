// Written by Nick Gammon
// April 2011
#include <SPI.h>

volatile unsigned char SPI_buff[100];
volatile byte cnt = 0;
volatile byte pos = 0;
volatile byte resp = 0;
volatile byte incoming = 0;
volatile byte outgoing = 0;
volatile unsigned char resp_buff[11];

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
  
  //If we are receiving a packet
  if(incoming){
    cnt++;
    //Serial.println(SPDR);
    SPI_buff[pos] = SPDR;
    pos++;
    if(cnt >= 11){
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
  if(SPDR == 0xFF && outgoing){
    SPDR = 'Y';
  }
  
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  //Received a full packet, lets print it
  //We should also get our response ready!
  if(cnt == 11){  
    //Print out the packet
    Serial.println(pos);
    byte ind = (pos-11)%100;
    for(int i = 0; i < 11; i++){
      
      if(ind > 99){
        ind = 0;
      }
      //Serial.println(ind);
      Serial.write(SPI_buff[ind]);
      
      ind++;
    }
    Serial.print("\n");
    cnt = 0;
    
    //Ready our response!
    
  }
}
