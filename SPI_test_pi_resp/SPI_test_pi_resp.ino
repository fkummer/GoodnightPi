// Written by Nick Gammon
// April 2011
#include <SPI.h>

volatile unsigned char SPI_buff[99];
volatile byte cnt = 0;
volatile byte pos = 0;
volatile byte resp = 0;

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
  if(pos > 98){
    pos = 0;
  }
  
  cnt++;
  SPI_buff[pos] = SPDR;
  pos++;
  
  if(resp){
    SPDR = 'Y';
  }
  
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  //Received a packet
  if(cnt == 11 && !resp){  
    Serial.println(pos);
    for(int i = pos - 11; i < pos; i++){
      Serial.write(SPI_buff[i]);
    }
    Serial.print("\n");
    cnt = 0;
    resp = 1;
  }else{
    if(cnt == 11 && resp){
      //We sent back out our response, listen again
      Serial.println("We did it reddit");
      resp = 0;
      cnt = 0;
    }
  }
}  // end of loop
