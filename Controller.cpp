/////////////////////
// Controller.cpp
/////////////////////

#include "Controller.h"

/////////////////////////////////////////////////////

Controller::Controller(std::shared_ptr<Model> & model) {

	// Creates the model for the controller
	//Model conM (pathToController);
	controllerModel = model;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
}

void Controller::loadS() {
	// Create the shader to use for the controller
	GLint conS = LoadShaders(vertexShaderPath, fragShaderPath);
	controllerShader = conS;
}

glm::mat4 Controller::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

glm::vec3 Controller::GetColor() {
	return laser.color;
}

void Controller::Render(glm::mat4 view, glm::mat4 proj) {

	glUseProgram(controllerShader);

	// Calculate the toWorld matrix for the model
	model = glm::mat4(1.0f); 
	model = glm::translate(model, position);

	glm::quat orientation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
	glm::mat4 rotationMatrix = glm::toMat4(orientation);
	model *= rotationMatrix;
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::translate(model, glm::vec3(-0.35f, -0.50f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(controllerShader, "projection"), 1, GL_FALSE, &proj[0][0]);

	controllerModel->Draw(controllerShader);

	// Get the button presses
	if (inputState.Buttons & btn1) {
		// on button 1 press
	}else {
		
	}

	// Get the trigger presses
	if (inputState.IndexTrigger[hand] > .5) {
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
	ray.dir = glm::normalize(orientation * glm::vec3(0, 0, -1));
	ray.dist = 75.0f; //magic number - same as laserDist in Laser.h

}

