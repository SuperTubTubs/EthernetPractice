// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <netdb.h>
#include <stdlib.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include "SharedHeader.h"
#include "DaemonHeader.h"


NetworkDaemon* NetworkDaemon::instance = NULL;


NetworkDaemon * NetworkDaemon::getInstance()
{
	if (instance == NULL) {
		instance = new NetworkDaemon;
	}
	return instance;
}


void NetworkDaemon::setupBroadcastSocket() {
	printf("Setting up socket\n");
	if ((networkSocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 0) {
		printf("Error createing broadcast socket\n");
		exit(EXIT_FAILURE);
	}
	int broadcastPermission = 1;
	if (setsockopt(networkSocketID, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
		printf("Error createing broadcast socket options\n");
		exit(EXIT_FAILURE);
	}
	
	memset(&socketAddress, 0, sizeof(socketAddress));        /* Zero out structure */

	socketAddress.sin_family = AF_INET;                      /* Internet address family */




	socketAddress.sin_addr.s_addr = inet_addr("192.168.1.255");


	socketAddress.sin_port = htons(PORT);

	this->bindSocket();
}

void NetworkDaemon::createBaseSocket() {
	if ((networkSocketID = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 0) {
		printf("Error createing socket\n");
		exit(EXIT_FAILURE);
	}
	int optionValue = 1;
	if (setsockopt(networkSocketID, SOL_SOCKET, SO_REUSEADDR, (void *)&optionValue, sizeof(optionValue)) < 0) {
		printf("Error createing socket options\n");
		exit(EXIT_FAILURE);
	}

	memset(&socketAddress, 0, sizeof(socketAddress));        /* Zero out structure */

	socketAddress.sin_family = AF_INET;                      /* Internet address family */

	

#ifdef SERVER_SOCKET
	socketAddress.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);
#else
	socketAddress.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);
#endif

	socketAddress.sin_port = htons(PORT);                   
}

void NetworkDaemon::setupClientSocket() {
	
	this->createBaseSocket();
	this->bindSocket();
	this->setMulticastOptions();
}

void NetworkDaemon::setupServerSocket() {
	this->createBaseSocket();
	//this->bindSocket();
}

void NetworkDaemon::bindSocket() {
	printf("binding socket\n");

	if (bind(networkSocketID, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0)
	{
		perror("bind failed.\n");
		exit(EXIT_FAILURE);
	}

}

void NetworkDaemon::setMulticastOptions() {
	/* use setsockopt() to request that the kernel join a multicast group */

	printf("setting multicast options.\n");
	ipMulticastData.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
	ipMulticastData.imr_interface.s_addr = htonl(INADDR_ANY);
	printf("after data set\n");

	if (setsockopt(networkSocketID, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipMulticastData, sizeof(ipMulticastData)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	u_char ttlOption = '255';
	if (setsockopt(networkSocketID, IPPROTO_IP, IP_MULTICAST_TTL, &ttlOption, sizeof(ttlOption)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	printf("after if statment\n");
}

void NetworkDaemon::connectSocket() {
	printf("connecting to socket \n");
	

	if (connect(networkSocketID, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0)
	{
		printf("\nConnection Failed \n");
		//return -1;
	}
}

void NetworkDaemon::receiveData() {
	socklen_t recieveSocketLength = sizeof(socketAddress);
	int recieveData;
	printf("receiving...");
	receiveMessageLength = recvfrom(networkSocketID, &recieveData, sizeof(recieveData), 0, (struct sockaddr *)&socketAddress, &recieveSocketLength);

	printf("Here is the message length = %d\n", receiveMessageLength);
	if (receiveMessageLength <= 0) {
		printf("\nError in receiving message length = %d\n", receiveMessageLength);
	}
	else {
		printf("Here is the recieved data: \n");
		printf("recieved tempNumber = %d\n", recieveData);
	}

}

void NetworkDaemon::sendData(int data) {

	int numberOfBytesSent = sendto(networkSocketID, (void *) &data, sizeof(data), 0, (struct sockaddr *)&socketAddress, sizeof(socketAddress));

	if (numberOfBytesSent <= 0) {
		printf("\nError in sending message length = %d\n", receiveMessageLength);
	}
	else {
		printf("Here is the sent data: \n");
		printf("sent tempNumber = %d\n", data);
	}

}