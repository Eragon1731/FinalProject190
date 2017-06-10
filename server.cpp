#include "rpc\server.h"
#include <iostream>
#include <vector>


using namespace std; 

//projections and headpose for clients

int clientIDs[5] = {-1, -1, -1,-1,-1}; 

struct Client
{
	vector<float> pose;
	//float position[16] = { 0 }; 
	int id; 
	int gamestate;
	int moleculeid; 

	bool resetState = false; 
} Client;

struct Client client1;
struct Client client2; 

//assignID ok
int assignID(int i) {
	
	if (i == 0) {
		client1.id = i; 
	}
	if (i == 1) {
		client2.id == i; 
	}

	clientIDs[i] = i; 
	cout << "client id: " << clientIDs[i] << endl; 
	return clientIDs[i];
}

//NEED to test
int quitGame(int id) {
	
	clientIDs[id] = -1; 
	return clientIDs[id];
}

//setting position
void clientPosition(int id, float pX, float pY, float pZ) {

	//store projection
	if (id == client1.id) {
		client1.pose.push_back(pX);
		client1.pose.push_back(pY);
		client1.pose.push_back(pZ);
	
	}
	else if (id == client2.id) {
		client2.pose.push_back(pX);
		client2.pose.push_back(pY);
		client2.pose.push_back(pZ);
	}

}

//recieving positions from struct  
float recievePosition(int id, int count) {

	if (client1.pose.empty() == true) {
		client1.pose.assign(3,0.0f);
		printf("client0 empty\n");
		return 0;
	}
	else if (client2.pose.empty() == true) {
		client2.pose.assign(3, 0.0f);
		printf("client1 empty\n");
		return 0;
	}

	if (id == client1.id && (!client2.pose.empty())) {
		//printf("returning observer for 0\n: %f", client2.pose[count]);
		return client2.pose[count];
	}
	else if (id == client2.id && (!client1.pose.empty())) {
		printf("returning observer for 1\n: %f",client1.pose[count]);
		return client1.pose[count];
	}
}

//setting win or loose state 
void setWinState(int id, int winState) {
	if (id == 0) {
		client1.gamestate = winState;
	}
	if (id == 1) {
		client2.gamestate = winState; 
	}
}

//return opponent's state 
int gameWin(int id) {

	if (id == 0) {
		return client2.gamestate; 
	}
	if (id == 1) {
		return client1.gamestate; 
	}
}

//setting moleculeID
void setMoleculeShot(int id, int mID) {
	if (id == 0) {
		client1.moleculeid = mID;
	}
	if (id == 1) {
		client2.moleculeid = mID; 
	}
}

//returning moleculeID
int moleculeShot( int id) {

	if (id == 0) {
		return client2.moleculeid; 
	}
	else if (id == 1) {
		return client1.moleculeid; 
	}
}

//setting reset state
void setGameReset(int id, bool resetFlag) {
	if (id == 0) {
		client1.resetState = resetFlag;
		cout << "0 resetState: " << client1.resetState << endl;
	}
	if (id == 1) {
		client2.resetState = resetFlag; 
		cout << "1 resetState: " << client2.resetState << endl;
	}
}

//returns resetState
bool gameReset(int id) {

	if (id == 0) {
		//cout << "0 returning state of 1: " << client2.resetState << endl;
		return client2.resetState;
	}
	if (id == 1) {
		//cout << "1 returning state of 0: " << client1.resetState << endl;
		return client1.resetState; 
	}
}

int main() {
	rpc::server srv(8080);


	srv.bind("assignID", &assignID); 

	//observer position 
	srv.bind("recievePosition", &recievePosition);
	srv.bind("clientPosition", &clientPosition); 

	//game state
	srv.bind("setWinState", &setWinState);
	srv.bind("gameWin", &gameWin);

	//molecule shot; ID init
	srv.bind("setMoleculeShot", &setMoleculeShot);
	srv.bind("moleculeShot", &moleculeShot);

	//game reset
	srv.bind("setGameReset", &setGameReset);
	srv.bind("gameReset", &gameReset);

	srv.run(); 
	return 0;
}
