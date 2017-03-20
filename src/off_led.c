#include <stdio.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LED	4

int main (void)
{
  float delayTime;
  int i;

  wiringPiSetup () ;
  pinMode (LED, OUTPUT) ;	

  digitalWrite (LED, LOW) ;	// On

}
