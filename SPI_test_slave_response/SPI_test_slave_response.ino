// Written by Nick Gammon
// April 2011

// what to do with incoming data
volatile byte command[11];


void setup (void)
{
  Serial.begin(9600);
  
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
  static unsigned char index;

  if(index > 10)
    index = 0;

  command[index] = SPDR;

  Serial.print(command[index], DEC);
  
  index++;
  
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  
  // if SPI not active, clear current command
  if (digitalRead (SS) == HIGH)
  {
    for(int j = 0; j<11; j++)
      command[j] = 0;
  }
}  // end of loop
