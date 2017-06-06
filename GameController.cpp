/////////////////////
// Controller.cpp
/////////////////////

#include "GameController.h"

/////////////////////////////////////////////////////

GameController::GameController() {

	// Creates the model for the controller
	Model conM (pathToController);
	controllerModel = conM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);

	//check controller
	if(controller.isConnected()){
		Frame frame = controller.frame();
		Frame previous = controller.frame(1); 
	}
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
	return laser.color;
}

void GameController::Render(glm::mat4 view, glm::mat4 proj) {
	//get hands?
	frame = controller.frame();
	hands = frame.hands();
	leftHand = hands.leftmost(); 
	rightHand = hands.rightmost(); 
	//
	glUseProgram(controllerShader);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, position);

	////glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	//glm::mat4 rotationMatrix = leftHand.rotationMatrix().toMatrix3x3();
	//model *= rotationMatrix;
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "projection"), 1, GL_FALSE, &proj[0][0]);

	controllerModel.Draw(controllerShader);

	// hand open 
	if (leftHand.grabAngle() == 0) {
		laser.SetRed();
	}
	else {
		laser.SetGreen();
	}

	// Render the laser
	laser.position = position;
	laser.rotation = rotation;
	laser.Render(view, proj);

	// Calculates the ray equation
	ray.origin = position;
	ray.dir = glm::normalize(leftHand.palmNormal * glm::vec3(0, 0, -1));
	ray.dist = 75.0f; //magic number - same as laserDist in Laser.h

}

