#include "ServerNetwork.h"

ServerNetwork::ServerNetwork() {

	WSADATA wsaData;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	struct addrinfo * result = NULL;
	struct addrinfo hints; 

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(1);
	}

	ZeroMemory(&hints, sizeof(hints)); 
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; 

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult); 
		WSACleanup();
		exit(1);
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup(); 
		exit(1);
	}

	u_long iMode = 1;
	iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

	if (iResult == SOCKET_ERROR) {
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError()); 
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}

	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
	
		printf("bind failed with error: %d\n", WSAGetLastError()); 
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		exit(1); 
	}

	freeaddrinfo(result); 

	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
	
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		exit(1);
	}
}

bool ServerNetwork::acceptNewClient(unsigned int & id) {

	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket != INVALID_SOCKET) {
	
		char value = 1;
		setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

		sessions.insert(pair<unsigned int, SOCKET>(id, ClientSocket)); 

		return true; 
	}
	return false; 
}

int ServerNetwork::recieveData(unsigned int client_id, char* recvbuf) {

	if (sessions.find(client_id) != sessions.end()) {
	
		SOCKET currentSocket = sessions[client_id];
		iResult = NetworkServices::recieveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE); 

		if (iResult == 0) {
			printf("Connection closed \n");
			closesocket(currentSocket);

		}
		return iResult; 
	}
	return 0; 
}

void ServerNetwork::sendToAll(char * packets, int totalSize) {

	SOCKET currentSocket; 

	map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;

	for (iter = sessions.begin(); iter != sessions.end(); iter++) {

		currentSocket = iter->second;
		iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);

		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError()); 
			closesocket(currentSocket); 

		}
	}

}