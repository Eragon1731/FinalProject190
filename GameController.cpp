/////////////////////
// Controller.cpp
/////////////////////

#include "GameController.h"

/////////////////////////////////////////////////////
using namespace Leap;

GameController::GameController() {

	// Creates the model for the controller
	Model conM (pathToController);
	controllerModel = conM;
	controllerTextureID = conM.textureID; 
	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);

	loadS(); 
	//load texture maps 
	vector<const GLchar * > textureFile;
	for (int i = 0; i < 6; i++) {
		textureFile.push_back("../objects/vr_test_pattern.ppm");
	}

	conM.loadTexturemap(textureFile); 
}

void GameController::loadS() {
	// Create the shader to use for the controller
	GLint conS = LoadShaders(vertexShaderPath, fragShaderPath);
	controllerShader = conS;
}

glm::mat4 GameController::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

glm::vec3 GameController::GetColor() {
	return glm::vec3(0.0f,1.0f,0.0f); //laser.color;
}

void GameController::Render(glm::mat4 view, glm::mat4 proj) {

	glUseProgram(controllerShader);

	//Calculate the toWorld matrix for the model

	frame = control.frame();
	hands = frame.hands();
	firsthand = hands[0];
	
	glm::vec3 tempPose = glm::vec3(firsthand.palmPosition().x, firsthand.palmPosition().y, firsthand.palmPosition().z); 
	glm::vec3 tempNormal = glm::vec3(firsthand.palmNormal().x, firsthand.palmNormal().y, firsthand.palmNormal().z); 

	tempPose = tempPose * 0.01f; 

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 0.7f, tempNormal);
	toWorld = glm::translate(glm::mat4(1.0f), tempPose);

	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "model"), 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "projection"), 1, GL_FALSE, &proj[0][0]);

	////Texture render
	//glActiveTexture(GL_TEXTURE0); // diff 
	//glUniform1i(glGetUniformLocation(controllerShader, "cube"), 0);//diff
	//glBindTexture(GL_TEXTURE_CUBE_MAP, controllerTextureID); //diff

	controllerModel.Draw(controllerShader);

	// Get the trigger presses
	if (renderLaser) {
		laser.SetGreen();

		// Render the laser
		laser.position = tempPose;
		laser.rotation = glm::vec4(tempNormal, 1.0f);
		laser.Render(view, proj);

	//	Calculates the ray equation
		ray.origin = position;
		ray.dir = glm::normalize(tempPose * glm::vec3(0, 0, -1));
		ray.dist = 75.0f; //magic number - same as laserDist in Laser.h

		cout << "render laser" << endl; 
	}
}

