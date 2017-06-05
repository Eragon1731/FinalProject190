#pragma once
#include <WinSock2.h>
#include <Windows.h>

class NetworkServices {

public:
	static int sendMessage(SOCKET curSocket, char * message, int messageSize);
	static int recieveMessage(SOCKET curSocket, char * buffer, int bufferSize);
};