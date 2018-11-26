// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "SharedHeader.h"
#include "DaemonHeader.h"


int main(int argc, char const *argv[]) 
{ 
	NetworkDaemon *networkDaemon = NetworkDaemon::getInstance();
	networkDaemon->setupBroadcastSocket();
	#ifdef SERVER_SIDE
		networkDaemon->bindSocket();
	#endif
	tempData dataPacket;

	#ifndef SERVER_SIDE
		networkDaemon->connectSocket();
	#endif 

	int counter = 0;
	while (true)
	{

		if (counter == 2000) {
			dataPacket.tempChar = 'F';
			dataPacket.tempNumber = 32;
		}

		if (counter == 4000) {
			#ifndef SERVER_SIDE
				networkDaemon->receiveFromSocket();
			#else
				networkDaemon->sendToSocket(&dataPacket);
			#endif
			counter = 0;
		}
		counter++;
	}
} 