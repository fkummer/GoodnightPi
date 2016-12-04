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
  int repeat = 1;
  
  long timeInterval;//Did a timer wake us?
  unsigned char int0;//Did an interrupt wake us?
  unsigned char int1;
  
  while(repeat){
	  sleep(5);
	  sleepState = digitalRead(SLEEP_GPIO);
	  //printf("hi\n");

	  if(sleepState){
		unsigned char resp[11];
		wakeRequest(resp);
		
		//It is a wake response
		if(resp[0] == 0x01){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",resp[i]);
			}
			printf("\n");
			
			timeInterval = 0;
			int0 = 0;
			int1= 0;
			
			//Put the 4 bytes together to recover the time
			int k;
			for( k = 4; k > 0; k--){
				printf("%i", resp[k]);
				timeInterval = timeInterval | resp[k];
				if(k > 1){
					timeInterval = timeInterval << 8;
				}
			}
			
			int0 = resp[5];
			int1 = resp[6];
			
			//If a timer set us off
			if(timeInterval != 0){
				printf("Timer\n");
				printf("%ld\n", timeInterval);
				repeat = 0;
				system("sudo python /home/pi/final/GoodnightPi/Timer.py");
			}else{
				//If interrupt 0 set us off
				if(int0){
					printf("Int0\n");
					repeat = 0;
					system("sudo python /home/pi/final/GoodnightPi/INT0.py");
				}else{
					//If interrupt 1 set us off
					if(int1){
						printf("Int1\n");
						repeat = 0;
						system("sudo python /home/pi/final/GoodnightPi/INT1.py");
					}else{
						printf("Mystery...\n");
					}
				}
			}
			
		}else{
			printf("Wake Fail\n");
		}
	  }
  }
  
  printf("Finito\n");
  unsigned char sleepResp[11];
  sleepRequest(sleepResp);
  while(sleepResp[0] != 0x03){
	  sleepRequest(sleepResp);
  }
  //We're good to shutdown!
  
  printf("Grande Finito!\n");
  system("sudo shutdown -h now");
}
