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

using namespace std;

// channel is the wiringPi name for the chip select (or chip enable) pin.
// Set this to 0 or 1, depending on how it's connected.
static const int CHANNEL = 1;



unsigned char transferAndWait(unsigned char msg){
	wiringPiSPIDataRW(CHANNEL, &msg, 1);
	delayMicroseconds(20);
}

void clearBuffer(unsigned char * buff, int len){
   int j;
   for(j = 0; j < len; j++){
		buff[j] = 0;
   }
}

void copyBuffer(unsigned char * dest, unsigned char * src, int len){
   int i;
   for(i = 0; i < len; i++){
       dest[i] = src[i];
   }
}

void transferPacket(unsigned char * buff, int len){
	int i = 0;
	for(i = 0; i < len; i++){
		transferAndWait(buff[i]);
	}
}

void receivePacket(unsigned char * buff, int len){
	int i = 0;
	//Create junk to send
	for(i = 0; i < len; i++){
		buff[i] = 0xFF;
	}
	
	//Do this nice and easy, one byte at a time.
	int j = 0;
	for(j = 0; j < len; j++){
		//Each is a "buffer" of size 1 that gets overwritten
		transferAndWait(buff[i]);
	}
	
}

int main()
{
   int fd, result;
   unsigned char string[] = {'a','b','c','d','e','f','g','h','i','j'};
   unsigned char sendBuffer[11];
   unsigned char receiveBuffer[11];
   
   clearBuffer(sendBuffer, 11);
   clearBuffer(receiveBuffer, 11);
   
   cout << "Initializing" << endl ;

   // Configure the interface.
   // CHANNEL insicates chip select,
   // 500000 indicates bus speed.
   fd = wiringPiSPISetup(CHANNEL, 500000);

   cout << "Init result: " << fd << endl;
   
   
   while(1){
	   copyBuffer(sendBuffer, string, 11);
	   cout << "Sending" << endl;
	   cout << sendBuffer << endl;
	   transferPacket(sendBuffer, 11);
	   sleep(5);
	   receivePacket(receiveBuffer, 11);
	   cout << "Received" << endl;
	   cout << receiveBuffer << endl;
	   sleep(5);
	   
	   
	   
   }

}
