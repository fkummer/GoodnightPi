//Ok, the other script has put out the alive GPIO
//Now it's our job to:
//1. Determine if sleeping is activated by checking the sleep GPIO
//2. Determine what woke me up
//3. Based on what woke me up, take the appropriate action by calling the right script
//
//That's the whole job of this script, the other script is in charge of handling shutdown

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdlib.h>

#include "raspi_spi.h"


#define	SLEEP_GPIO 5 //BROADCOM 24 = WiringPi 5
static const int CHANNEL = 1;

int main(){
	
  wiringPiSetup();
  wiringPiSPISetup(CHANNEL, 500000);
  pinMode (SLEEP_GPIO, INPUT) ;	
  pullUpDnControl (SLEEP_GPIO, PUD_OFF);
  
  int sleepState = digitalRead(SLEEP_GPIO);
  
  sleepState = digitalRead(SLEEP_GPIO);
  printf("%i\n", sleepState);

}
