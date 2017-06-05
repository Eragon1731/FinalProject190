#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <WS2tcpip.h>
#include <stdio.h>

//buffer size
#define DEFAULT_BUFLEN 512
//port to connect sockets
#define DEFAULT_PORT "6881"
//need to link libs
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork {

public:
	int iResult;
	SOCKET ConnectSocket;

	ClientNetwork(void);
	~ClientNetwork(void); 

	int receivePackets(char*); 
};
