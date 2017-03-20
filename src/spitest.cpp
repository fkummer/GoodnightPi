/******************************************************************************
Byron Jacquot @ SparkFun Electronics>
4/2/2014
https://github.com/sparkfun/Pi_Wedge

A brief demonstration of the Raspberry Pi SPIinterface, using the SparkFun
Pi Wedge breakout board.

Resources:

This example makes use of the Wiring Pi library, which streamlines the interface
to the the I/O pins on the Raspberry Pi, providing an API that is similar to the
Arduino.  You can learn about installing Wiring Pi here:
http://wiringpi.com/download-and-install/

The wiringPi SPI API is documented here:
https://projects.drogon.net/raspberry-pi/wiringpi/spi-library/

The init call returns a standard file descriptor.  More detailed configuration
of the interface can be performed using ioctl calls on that descriptor.
See the wiringPi SPI implementation (wiringPi/wiringPiSPI.c) for some examples.
Parameters configurable with ioctl are documented here:
http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/Documentation/spi/spidev

Hardware connections:

This file interfaces with the SparkFun Serial 7 Segment display:
https://www.sparkfun.com/products/11629

The board was connected as follows:
(Raspberry Pi)(Serial 7 Segment)
GND  -> GND
3.3V -> Vcc
CE1  -> SS (Shift Select)
SCK  -> SCK 
MOSI -> SDI
MISO -> SDO

To build this file, I use the command:
>  g++ spitest.cpp -lwiringPi

Then to run it, first the spi kernel module needs to be loaded.  This can be 
done using the GPIO utility.
> gpio load spi
> ./a.out

This test uses the single-segment mode of the 7 segment display.  It shifts a 
bit through the display characters, lighting a single character of each at a 
time.

Development environment specifics:
Tested on Raspberry Pi V2 hardware, running Raspbian.
Building with GCC 4.6.3 (Debian 4.6.3-14+rpi1)

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <iostream>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <stdio.h>

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
	
	//cout << "junked" << endl;
	//cout << buff << endl;
	
	//Do this nice and easy, one byte at a time.
	unsigned char j = 0;
	for(j = 0; j < len; j++){
		//Each is a "buffer" of size 1 that gets overwritten
		//printf("%d",j);
		//cout << buff[j] << endl;
		transferAndWait(&buff[j]);
		
		
	}
	
	//cout << "post-junked" << endl;
	//cout << buff << endl;
	
	//unsigned char k = 0;
	//for(k =0; k < len; k++){
	//	buff[k] = buff [k+11];
	//}
	//buff[11] = '\0';
}

int main()
{
   int fd, result;
   
   unsigned char string[] = {0x00,'b','c','d','e','f','g','h','i','j','\0'};
   
   cout << string << endl; 
   
   printf("Initializing\n");

   // Configure the interface.
   // CHANNEL insicates chip select,
   // 500000 indicates bus speed.
   fd = wiringPiSPISetup(CHANNEL, 500000);
   
   while(1){
	   unsigned char sendBuffer[11];
	   unsigned char receiveBuffer[11];
	   
	   
	   clearBuffer(sendBuffer, 11);
	   clearBuffer(receiveBuffer, 11);
	   
	   string[0] = (string [0] + 0x02)%0x08; 
	   copyBuffer(sendBuffer, string, 11);
	  
	   
	   printf("Sending\n");
	   cout << sendBuffer << endl;
	   
	   unsigned char startByte = 0xFE;
	   
	   transferAndWait(&startByte);
	   
	   transferPacket(sendBuffer, 11);
	   
	   sleep(1);
	   
	   
	   receivePacket(receiveBuffer, 11);
	   
	   
	   printf("Received\n");
	   printf("OpCode: ");
	   printf("%#03x\n", receiveBuffer[0]);
	   cout << receiveBuffer << endl;
	   int k = 0;
	   ////for(k = 0;k<11;k++){
			////cout << receiveBuffer[k] << endl;
	   ////}
	   //cout << receiveBuffer<< endl;
	   sleep(1);
	   
	   
   }

}
