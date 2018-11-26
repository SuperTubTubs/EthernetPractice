#pragma once
#include <string>
#define PORT 8080

class NetworkDaemon {
public:
		static NetworkDaemon* getInstance();
		void setupSocket();
		void setupBroadcastSocket();
		void bindSocket();
		void connectSocket();
		void receiveFromSocket();
		void sendToSocket(tempData *);

		
private:
	NetworkDaemon() {};  // Private so that it can  not be called
	NetworkDaemon(NetworkDaemon const&) {};             // copy constructor is private
	static NetworkDaemon* instance;
	struct sockaddr_in socketAddress;
	int addressLength = sizeof(socketAddress);
	int networkSocket;
	int connectedSocket;
	const char* broadcastIP = "192.168.1.255";
	char * tempBuffer;
	int receiveMessageLength = 0;
	
};
