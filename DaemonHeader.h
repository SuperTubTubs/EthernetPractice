#pragma once
#include <string>
#define PORT 8080
#define MULTICAST_ADDRESS "225.0.0.37"

class NetworkDaemon {
public:
		static NetworkDaemon* getInstance();
		void createBaseSocket();
		void setupClientSocket();
		void setupServerSocket();
		void setupBroadcastSocket();
		void bindSocket();
		void connectSocket();
		void receiveData();
		void sendData(int);
		void setMulticastOptions();

		
private:
	NetworkDaemon() {};  // Private so that it can  not be called
	NetworkDaemon(NetworkDaemon const&) {};             // copy constructor is private
	static NetworkDaemon* instance;
	struct sockaddr_in socketAddress;
	int addressLength = sizeof(socketAddress);
	int networkSocketID;
	struct hostent* networkHostData;
	int connectedSocket;
	struct ip_mreq ipMulticastData;
	//const char* broadcastIP = "192.168.1.255";
	char * tempBuffer;
	int receiveMessageLength = 0;
	
};
