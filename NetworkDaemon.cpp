// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
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
	if ((networkSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 0) {
		printf("Error createing broadcast socket\n");
		exit(EXIT_FAILURE);
	}
	int broadcastPermission = 1;
	if (setsockopt(networkSocket, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
		printf("Error createing broadcast socket options\n");
		exit(EXIT_FAILURE);
	}
}

void NetworkDaemon::setupSocket() {
	if ((networkSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == 0) {
		printf("Error createing socket\n");
		exit(EXIT_FAILURE);
	}
	int broadcastPermission = 1;
	if (setsockopt(networkSocket, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *)&broadcastPermission, sizeof(broadcastPermission)) < 0) {
		printf("Error createing socket options\n");
		exit(EXIT_FAILURE);
	}
}

void NetworkDaemon::bindSocket() {
	printf("binding socket\n");
	memset(&socketAddress, 0, sizeof(socketAddress));     /* Zero out structure */
	socketAddress.sin_family = AF_INET;                      /* Internet address family */
	socketAddress.sin_addr.s_addr = inet_addr(broadcastIP);  /* Broadcast IP address */
	socketAddress.sin_port = htons(PORT);                    /* Broadcast port */

	if (bind(networkSocket, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0)
	{
		perror("bind failed.\n");
		exit(EXIT_FAILURE);
	}

}

void NetworkDaemon::connectSocket() {
	printf("connecting to socket \n");
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if (inet_pton(AF_INET, broadcastIP, &socketAddress.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		//return -1;
	}

	if (connect(networkSocket, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0)
	{
		printf("\nConnection Failed \n");
		//return -1;
	}
}

void NetworkDaemon::receiveFromSocket() {
	//if (listen(networkSocket, 3) < 0)
	//{
	//	perror("listen");
	//	exit(EXIT_FAILURE);
	//}
	//if (connectedSocket = accept(networkSocket, (struct sockaddr *)&socketAddress, (socklen_t*) &addressLength) < 0)
	//{
	//	perror("accept");
	//	exit(EXIT_FAILURE);
	//}
	socklen_t recieveSocketLength = sizeof(socketAddress);
	tempData recieveData;
	receiveMessageLength = recvfrom(networkSocket, &recieveData, tempDataSize, 0, (struct sockaddr *)&socketAddress, &recieveSocketLength);
	printf("Here is the message length = %d\n", receiveMessageLength);
	if (receiveMessageLength <= 0) {
		printf("\nError in receiving message length = %d\n", receiveMessageLength);
	}
	else {
		printf("Here is the recieved data: \n");
		printf("recieved tempNumber = %d\n", recieveData.tempNumber);
		printf("recieved tempChar = %c\n\n", recieveData.tempChar);
	}

}

void NetworkDaemon::sendToSocket(tempData * dataPacket) {

	int numberOfBytesSent = sendto(networkSocket, dataPacket, sizeof(struct tempData), 0, (struct sockaddr *)&socketAddress, sizeof(socketAddress));

	if (numberOfBytesSent <= 0) {
		printf("\nError in sending message length = %d\n", receiveMessageLength);
	}
	else {
		printf("Here is the sent data: \n");
		printf("sent tempNumber = %d\n", dataPacket->tempNumber);
		printf("sent tempChar = %c\n\n", dataPacket->tempChar);
	}

}