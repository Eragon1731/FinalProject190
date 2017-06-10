#include "rpc\server.h"
#include <iostream>
#include <vector>


using namespace std; 

//projections and headpose for clients

int clientIDs[5] = {-1, -1, -1,-1,-1}; 

struct Client
{
	vector<float> proj;
	vector<float> head; 
	int id; 
	bool gamestate;
	int moleculeid; 
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
	cerr << "client id: " << clientIDs[i] << endl; 
	return clientIDs[i];
}

//NEED to test
int quitGame(int id) {
	
	clientIDs[id] = -1; 
	return clientIDs[id];
}

//
void clientPosition(int id, float pX, float pY, float pZ, float pW) {

	//store projection
	if (id == client1.id) {
		client1.proj.push_back(pX);
		client1.proj.push_back(pY);
		client1.proj.push_back(pZ);
		client1.proj.push_back(pW);
	}
	else if (id == client2.id) {
		client2.proj.push_back(pX);
		client2.proj.push_back(pY);
		client2.proj.push_back(pZ);
		client2.proj.push_back(pW);
	}
	cerr << "id: " << id << "in" << endl; 
}

//sends back each float in vector
//column major 
float recievePosition(int id, int count) {
	if (id == client1.id)
		return client2.proj[count];
	else if (id == client2.id)
		return client1.proj[count]; 

}

bool gameWin(int id, bool winState) {

	if (id == 0) {
		if (winState == true) {
			return winState;
		}
		else
			return false; 
	}
}

//should be ok 
//NEED to check 
int moleculeShot( int id, int moleculeID) {
	if (id == 0) {
		client1.moleculeid = moleculeID;
		return client1.moleculeid; 
	}
	else if (id == 1) {
	//	client2.moleculeid = moleculeID;
		return client1.moleculeid; 
	}
}

int main() {


	rpc::server srv(8080);
	srv.bind("assignID", &assignID); 

	//projection and view
	srv.bind("clientPosition", &clientPosition); 
	/////

	srv.bind("recievePosition", &recievePosition);

	srv.bind("gameWin", &gameWin);
	srv.bind("moleculeShot", &moleculeShot);

	srv.run(); 
	return 0;
}
