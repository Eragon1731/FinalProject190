#include "rpc\server.h"
#include <iostream>
#include <vector>

#define MAX 50;

//int clientIDS [];
int id = -1; 


using namespace std; 
//char**
int assignID() {

	return id++;
}

int quitGame(int id) {
	//clientIDS[id] = -1;
}

vector<float> clientProjection(float rotX, float rotY, float rotZ, float posX, float posY, float posZ) {

	vector<float> projection; 

	projection.push_back(rotX);
	projection.push_back(rotY);
	projection.push_back(rotZ);
	projection.push_back(posX);
	projection.push_back(posY);
	projection.push_back(posZ); 

	return projection; 
}

//float * clientView(float rotX, float rotY, float rotZ, float posX, float posY, float posZ) {
//
//	float view[6];
//
//	view[0] = rotX;
//	view[1] = rotY;
//	view[2] = rotZ;
//	view[3] = posX;
//	view[4] = posY;
//	view[5] = posZ;
//
//	return view;
//}

bool gameWin(bool winState) {

	return winState;
}

int moleculeShot(int moleculeID) {

	return moleculeID; 
}

int main() {
	//clientIDS =  int[MAX]; 

	rpc::server srv(8080);
	srv.bind("assignID", &assignID); 

	//projection and view
	//srv.bind("clientProjection", &clientProjection); 
	//srv.bind("clientView", &clientView);
	/////

	srv.bind("gameWin", &gameWin);
	srv.bind("moleculeShot", &moleculeShot);

	srv.run(); 
	return 0;
}
