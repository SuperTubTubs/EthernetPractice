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
	
	#ifdef SERVER_SOCKET
		networkDaemon->setupServerSocket();
	#else
		networkDaemon->setupClientSocket();
	#endif // SERVER_SOCKET


	int counter = 0;
	while (true)
	{
		if (!(counter % 100)) {
			//usleep(250);
			usleep(50000);
		}

		if (!(counter % 10000)) {
			#ifdef SERVER_SOCKET
				networkDaemon->sendData(counter);
			#else
				networkDaemon->receiveData();
			#endif
		}
		counter++;
	}
} 