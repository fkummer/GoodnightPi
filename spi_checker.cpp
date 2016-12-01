

#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>
#include "raspi_spi.h"

#define START_BYTE 0xFE

using namespace std;

// channel is the wiringPi name for the chip select (or chip enable) pin.
// Set this to 0 or 1, depending on how it's connected.
static const int CHANNEL = 1;


unsigned char transferAndWait(unsigned char * msg){
	wiringPiSPIDataRW(CHANNEL, msg, 1);
	delayMicroseconds(5000);
}

void clearBuffer(unsigned char * buff, int len){
   unsigned char j;
   for(j = 0; j < len; j++){
		buff[j] = '\0';
   }
}

void copyBuffer(unsigned char * dest, unsigned char * src, int len){
   unsigned char i = 0;
   for(i = 0; i < len; i++){
       dest[i] = src[i];
   }
}                                            

void transferPacket(unsigned char * buff, int len){
	unsigned char i = 0;
	for(i = 0; i < len; i++){
		transferAndWait(&buff[i]);
	}
}


void receivePacket(unsigned char * buff, int len){
	unsigned char i = 0;
	//Create junk to send
	for(i = 0; i < len; i++){
		buff[i] = 0xFF;
	}
	
	//Do this nice and easy, one byte at a time.
	unsigned char j = 0;
	for(j = 0; j < len; j++){
		//Each is a "buffer" of size 1 that gets overwritten
		transferAndWait(&buff[j]);
	}
}

void sendRequest(unsigned char * request, unsigned char * response){
	unsigned char startByte = START_BYTE;
	
	 //Send message start byte, and then the packet
    transferAndWait(&startByte);
    transferPacket(request, 11);
   
    //Wait for the response to get itself together
    sleep(.1);
   
	//Get the response
    receivePacket(response, 11);
}

//Sends a WAKE_REQ and returns a pointer to the response.
void wakeRequest(unsigned char * wakeResp){
	unsigned char wakeReq[11];
	
	//Make sure no junk is hanging around in the arrays.
	clearBuffer(wakeReq, 11);
	clearBuffer(wakeResp, 11);
	
	//Assign opcode
	wakeReq[0] = 0x00;
	
	//wake requests have an empty payload
	int i;
	for(int i = 1;i < 11; i++){
		wakeReq[i]  = 0x00;
	} 
	
	sendRequest(wakeReq, wakeResp);
}

//Sends a SLEEP_REQ and returns a pointer to the response.
void sleepRequest(unsigned char * sleepResp){
	unsigned char sleepReq[11];
	
	//Make sure no junk is hanging around in the arrays.
	clearBuffer(sleepReq, 11);
	clearBuffer(sleepResp, 11);
	
	//Assign opcode
	sleepReq[0] = 0x02;
	
	//sleep requests have an empty payload
	int i;
	for(int i = 1;i < 11; i++){
		sleepReq[i]  = 0x00;
	} 
	
	sendRequest(sleepReq, sleepResp);
}

//Sends a CONFIG_REQ and returns a pointer to the response.
void configRequest(unsigned char * configResp, long timeInterval, unsigned char int0enable, unsigned char int0type, unsigned char int1enable, unsigned char int1type){
	unsigned char configReq[11];
	
	//Make sure no junk is hanging around in the arrays.
	clearBuffer(configReq, 11);
	clearBuffer(configResp, 11);
	
	//Assign opcode  fd = wiringPiSPISetup(CHANNEL, 500000);
	configReq[0] = 0x04;
	
	//4 bytes for time interval
	configReq[1] = (unsigned char)(timeInterval & 0x000000FF);
	configReq[2] = (unsigned char)(timeInterval & 0x0000FF00);
	configReq[3] = (unsigned char)(timeInterval & 0x00FF0000);
	configReq[4] = (unsigned char)(timeInterval & 0xFF000000);
	
	//1 byte to set interrupt 0
	configReq[5] = int0enable;
	
	//1 byte to set interrupt 0 type
	configReq[6] = int0type;
	
	//1 byte to set interrupt 1
	configReq[7] = int1enable;
	
	//1 byte to set interrupt 1 type
	configReq[8] = int1type;
	
	//2 bytes are empty
	configReq[9] = 0x00;
	configReq[10] = 0x00;
	
	sendRequest(configReq, configResp);
}

//Sends a PING_REQ and returns a pointer to the response.
void pingRequest(unsigned char * pingResp){
	unsigned char pingReq[11];
	
	//Make sure no junk is hanging around in the arrays.
	clearBuffer(pingReq, 11);
	clearBuffer(pingResp, 11);
	
	//Assign opcode
	pingReq[0] = 0x06;
	
	//ping requests have an empty payload
	int i;
	for(int i = 1;i < 11; i++){
		pingReq[i]  = 0x00;
	} 
	
	sendRequest(pingReq, pingResp);
}

int main()
{
   int fd, result;
   
   printf("Initializing\n");

   // Configure the interface.
   // CHANNEL insicates chip select,
   // 500000 indicates bus speed.
   fd = wiringPiSPISetup(CHANNEL, 500000);
   
   while(1){
		sleep(2);
		unsigned char result[11];
		
		printf("Wake\n");
		wakeRequest(result);
		if(result[0] == 0x01){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",result[i]);
			}
			printf("\n");
		}else{
			printf("Wake Fail\n");
		}
		sleep(2);
		
		printf("Sleep\n");
		sleepRequest(result);
		if(result[0] == 0x03){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",result[i]);
			}
			printf("\n");
		}else{
			printf("Sleep Fail\n");
		}
		sleep(2);
		
		printf("Config\n");
		configRequest(result,1000, 0, 0, 0, 0);
		if(result[0] == 0x05){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",result[i]);
			}
			printf("\n");
		}else{
			printf("Config Fail\n");
		}
		sleep(2);
		
		printf("Ping\n");
		pingRequest(result);
		if(result[0] == 0x07){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",result[i]);
			}
			printf("\n");
		}else{
			printf("Ping Fail\n");
		}
		sleep(2);
   }

}
