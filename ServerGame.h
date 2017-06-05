#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerGame {

public:
	ServerGame();
	~ServerGame();

	void update(); 
	void receiveFromClients(); 
	void sendActionPackets(); 

private:
	static unsigned int client_id; 
	ServerNetwork * network; 
	char network_data[MAX_PACKET_SIZE]; 
};