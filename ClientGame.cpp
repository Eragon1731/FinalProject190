#include "StdAfx.h"
#include "ClientGame.h"


ClientGame::ClientGame(void)
{

    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
} 



void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

//mine
void ClientGame::clientMove(int id, glm::mat4 projection, glm::mat4 headPose) {
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	//packet.packet_type = CLIENT_MOVE;
	packet.clientID = id; 
	packet.clientPosition = projection; 
	packet.clientHeadPose = headPose; 
	packet.serialize(packet_data);

	//NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::update()
{
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                sendActionPackets();

                break;
			case CLIENT_MOVE:
				printf("client %d moved", 0);
				sendActionPackets();

            default:

                printf("error in packet types\n");

                break;
        }
    }
}
