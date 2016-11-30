

#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>

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
		wakeRequest(result);
		if(result[0] == 0x01){
			int i;
			for(i = 1; i < 11; i++){
				printf("%c",result[i]);
			}
			printf("\n");
		}
		sleep(2);
   }

}
