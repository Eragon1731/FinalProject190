#include "rpc\server.h"
#include <iostream>
#include <vector>


using namespace std; 

//projections and headpose for clients

int clientIDs[5] = {-1, -1, -1,-1,-1}; 

struct Client
{
	float px = 0.0f;
	float py = 0.0f;
	float pz = 0.0f;

	int id; 
	int gamestate;
	int moleculeid; 

	bool resetState = false; 
} Client;

struct Client client1;
struct Client client2; 
//////////
float c2_x = 0.0f; 
float c2_y = 0.0f;
float c2_z = 0.0f; 

////////////
//assignID ok
int assignID(int i) {
	
	if (i == 0) {
		client1.id = i; 
		return client1.id; 
	}
	if (i == 1) {
		client2.id == i; 
		return client2.id; 
	}
}

//NEED to test
int quitGame(int id) {
	
	clientIDs[id] = -1; 
	return clientIDs[id];
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

void sendPosition(int id, float x, float y, float z) {
	if (id == client1.id) {
		client1.px = x;
		client1.py = y; 
		client1.pz = z; 
	}
	if (id == 1) {
		client2.px = x;
		c2_x = x; 
		client2.py = y;
		c2_y = y;
		client2.pz = z;
		c2_z = z; 
		cout << "client2 not empty: "<< c2_x <<" "<<c2_y<<" "<<c2_z<< endl;
	}
}
//for x
float positionX(int id) {
	//cout << "getting position: " << id << endl;
	if (id == client1.id && client1.px != 0) {
	//	cout << "client1 data" << endl;
		return client1.px;
	}
	else
		return 0; 
	cout << "client2 data before: " << client2.px << endl;
	if (id == client2.id && client2.px != 0) {
		cout << "getting position: " << id << endl;
		cout << "client2 data"<<client2.px << endl;
		return client2.px;
	}
	else
		return 0;
}
float positionY(int id) {

	if (id == client1.id && client1.py != 0)
		return client1.py;
	else if (id == client2.id && client2.py != 0)
		return client2.py;
	else
		return 0;
}
float positionZ(int id) {

	if (id == client1.id && client1.pz != 0)
		return client1.pz;
	else if (id == client2.id && client2.pz != 0)
		return client2.pz;
	else
		return 0;
}
//for leap->ocu only
float leapX(int id) {
	cout << "return " << id << " " << c2_x << endl;
	return c2_x;
}
float leapY(int id) {
	cout << "return " << id << " " << c2_y << endl;
	return c2_y;
}
float leapZ(int id) {
	cout << "return " << id << " " << c2_z << endl;
	return c2_z;
}


int main() {
	rpc::server srv(8080);

	srv.bind("assignID", &assignID); 

	//observer position 
	srv.bind("sendPosition", &sendPosition);
	srv.bind("positionX", &positionX);
	srv.bind("positionY", &positionY);
	srv.bind("positionZ", &positionZ);

	srv.bind("leapX", &leapX);
	srv.bind("leapY", &leapY);
	srv.bind("leapZ", &leapZ);
	///////
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
