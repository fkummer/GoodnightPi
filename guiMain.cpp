#include <iostream>
#include <stdio.h>
#include "raspi_spi.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <fstream>

using namespace std;

#define	SLEEP_GPIO 5 //BROADCOM 24 = WiringPi 5
static const int CHANNEL = 1;

int main(int argc, char* argv[])
{
	unsigned char arr[11];
	long timeInterval;
	
	wiringPiSetup();
	wiringPiSPISetup(CHANNEL, 500000);
	pinMode (SLEEP_GPIO, INPUT) ;	
	pullUpDnControl (SLEEP_GPIO, PUD_OFF);
	
	
	if(strcmp(argv[1],"ping") == 0)
	{
		cout<<"ping"<<endl;
		pingRequest(arr);
	}
	
	if(strcmp(argv[1],"interrupt0") == 0)
	{
		cout<<"interrupt0"<<endl;
		configRequest(arr, 0, 1, 1, 0, 0);
	}
	
	if(strcmp(argv[1],"interrupt1") == 0)
	{
		cout<<"interrupt1"<<endl;
		configRequest(arr, 0, 0, 0, 1, 1);
	}
	
	if(strcmp(argv[1],"interval") == 0)
	{
		cout<<"interval"<<endl;
		
		if(strcmp(argv[2],"60") == 0)
		{
			timeInterval = 60;
		}
		else if(strcmp(argv[2],"120") == 0)
		{
			timeInterval = 120;
		}
		else if(strcmp(argv[2],"300") == 0)
		{
			timeInterval = 300;
		}
		else if(strcmp(argv[2],"600") == 0)
		{
			timeInterval = 600;
		}
		else if(strcmp(argv[2],"900") == 0)
		{
			timeInterval = 900;
		}
		else if(strcmp(argv[2],"1800") == 0)
		{
			timeInterval = 1800;
		}
		else if(strcmp(argv[2],"3600") == 0)
		{
			timeInterval = 3600;
		}
		else if(strcmp(argv[2],"7200") == 0)
		{
			timeInterval = 7200;
		}
		else if(strcmp(argv[2],"18000") == 0)
		{
			timeInterval = 18000;
		}
		else if(strcmp(argv[2],"36000") == 0)
		{
			timeInterval = 36000;
		}
		else if(strcmp(argv[2],"54000") == 0)
		{
			timeInterval = 54000;
		}
		else if(strcmp(argv[2],"72000") == 0)
		{
			timeInterval = 72000;
		}
		else if(strcmp(argv[2],"86400") == 0)
		{
			timeInterval = 86400;
		}
		
		configRequest(arr, timeInterval, 0, 0, 0, 0);
		
	}
	
	switch(arr[0])
	{
		case 0x05: cout<<"success"<<endl;
					break;
		
		case 0x07: cout<<"ping success"<<endl;
				    ofstream myfile;
					myfile.open("ping.txt");
					myfile<<"Ping Success!";
					myfile.close();
					break;
	
	}
	
}


