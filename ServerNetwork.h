#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <WS2tcpip.h>
#include <map>

using namespace std; 

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

class ServerNetwork {

public:
	ServerNetwork();
	~ServerNetwork(); 
	bool acceptNewClient(unsigned int & id); 
	int recieveData(unsigned int client_id, char * recvbuf); 
	void sendToAll(char* packets, int totalSize); 

	SOCKET ListenSocket;
	SOCKET ClientSocket;

	int iResult;

	map<unsigned int, SOCKET> sessions; 
};