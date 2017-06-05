#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame {

public:
	ClientGame();
	~ClientGame(); 

	void sendActionPackets();
	char network_data[MAX_PACKET_SIZE];
	void update(); 

	ClientNetwork * network; 
};