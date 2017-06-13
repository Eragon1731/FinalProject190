#include "Observer.h"

Observer::Observer() {

	observerWorld = glm::mat4(1.0f); 
	observerWorld = glm::scale(observerWorld, glm::vec3(2.0f,2.0f,2.0f));
	// Creates the model for the controller
	Model conM(pathToObserver);
	observerModel = conM;
	loadS();
	vector<const GLchar* > faces;
	for (int i = 0;i < 6;i++)
		faces.push_back("../objects/vr_test_pattern.ppm");

	observerModel.loadTexturemap(faces);
	otherPosition = glm::vec3(0.0f,0.0f,0.0f); 
}

void Observer::loadS() {
	// Create the shader to use for the controller
	GLint conS = LoadShaders(vertexShaderPath, fragShaderPath);
	observerShader = conS;
}

void Observer::Render(glm::mat4 view, glm::mat4 proj) {

	glUseProgram(observerShader);

	//Calculate the toWorld matrix for the model

	observerWorld = glm::translate(glm::mat4(1.0f), otherPosition);

	glUniformMatrix4fv(glGetUniformLocation(observerShader, "model"), 1, GL_FALSE, &observerWorld[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(observerShader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(observerShader, "projection"), 1, GL_FALSE, &proj[0][0]);

	////Texture render
	//glActiveTexture(GL_TEXTURE0); // diff 
	//glUniform1i(glGetUniformLocation(controllerShader, "cube"), 0);//diff
	//glBindTexture(GL_TEXTURE_CUBE_MAP, controllerTextureID); //diff

	observerModel.Draw(observerShader);
}

