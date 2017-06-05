#include "ClientNetwork.h"

ClientNetwork::ClientNetwork() {

	WSADATA wsaData;

	ConnectSocket = INVALID_SOCKET;
	struct addrinfo * result = NULL,
		*ptr = NULL,
		hints;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

	if (iResult != 0) {

		printf("getaddringo failed with error: %d\n", iResult);
		WSACleanup();
		exit(1);
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			exit(1);
		}

		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("server is down...did not connect");
		}
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		exit(1); 
	}

	char value = 1;
	setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)); 
}

int ClientNetwork::receivePackets(char * recvbuf) {

	iResult = NetworkServices::recieveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE); 

	if(iResult == 0){
	
		printf("connection closed \n");
		closesocket(ConnectSocket); 
		WSACleanup(); 
		exit(1);
	}
	return iResult; 
}