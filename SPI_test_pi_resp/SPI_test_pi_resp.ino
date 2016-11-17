// Written by Nick Gammon
// April 2011

// what to do with incoming data
volatile byte command = 0;

unsigned char SPI_buff[11];
volatile byte pos = 0;

void setup (void)
{
  Serial.begin(115200);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  if(pos < 11){
    SPI_buff[pos] = SPDR;
    pos = pos + 1;
  }else{
     pos = 0;
     for(int i = 0; i < 11; i++){
        Serial.print(SPI_buff[i]);
     }
     Serial.println('');
    
  }

}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  
}  // end of loop
