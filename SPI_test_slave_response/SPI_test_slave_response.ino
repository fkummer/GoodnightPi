// Written by Nick Gammon
// April 2011

// what to do with incoming data
volatile byte command = 0;

void setup (void)
{
  Serial.begin(115200)
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
  byte c = SPDR;
  Serial.println(c);
  SPDR = 'a';

}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  
  // if SPI not active, clear current command
  if (digitalRead (SS) == HIGH)
    command = 0;
}  // end of loop
