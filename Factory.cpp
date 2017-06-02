/////////////////////
// Factory.cpp
/////////////////////

#include "Factory.h"

/////////////////////////////////////////////////////

Factory::Factory() {

	// Create the shader to use for the factory
	GLint facS = LoadShaders(vertexShaderPath, fragShaderPath); 
	factoryShader = facS;

	
	// Creates the model for the factory
	Model facM(pathToFactory);
	factoryModel = facM;

	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	
}

glm::mat4 Factory::GetModelMatrix() {
	glm::mat4 modelMat = glm::translate(position);
	return modelMat;
}

void Factory::Render(glm::mat4 view, glm::mat4 proj) {

	glUseProgram(factoryShader);

	// Calculate the toWorld matrix for the model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -0.75f, -1.5f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(factoryShader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(factoryShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(factoryShader, "projection"), 1, GL_FALSE, &proj[0][0]);


	factoryModel.Draw(factoryShader);
//	cerr << "can factory" << endl;
}

