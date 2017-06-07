#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	ClientNetwork* network;

	void sendActionPackets();
	//test
	/*void encodeData(glm::mat4 projection, glm::mat4 headPose); */
	void clientMove(int id, glm::mat4 projection, glm::mat4 headPose); 
	void addClientID(int id); 
	///
    char network_data[MAX_PACKET_SIZE];

    void update();
};

