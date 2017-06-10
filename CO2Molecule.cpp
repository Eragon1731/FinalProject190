/////////////////////
// CO2Molecule.cpp
/////////////////////

#include "CO2Molecule.h"

#define BOUND 15.0f

/////////////////////////////////////////////////////

CO2Molecule::CO2Molecule() {
	this->setup();
}

CO2Molecule::CO2Molecule(Model co2M, Model o2M, GLint shaderProgram) {
	co2Shader = shaderProgram;

	// Creates the model for the factory
	currentModel = co2M;
	co2Model = co2M;
	o2Model = o2M;

	random_vector = glm::normalize(glm::vec3(rand(), rand(), rand()));

	this->setup();
}

void CO2Molecule::setup() {
	// Sets the position / rotation / scale
	position = glm::vec3(0, 0, 0);
	rotation = 0.0f;
	scale = glm::vec3(1, 1, 1);
	color = glm::vec3(0.0f, 0.5f, 0.31f);
}

void CO2Molecule::ChangeToO2() {
	currentModel = o2Model;
	color = glm::vec3(1.0f, 1.0f, 1.0f);
}

void CO2Molecule::ChangeToCO2() {
	currentModel = co2Model;
	color = glm::vec3(0.0f, 0.5f, 0.31f);
}

void CO2Molecule::Render(glm::mat4 view, glm::mat4 proj) {

	glUseProgram(co2Shader); 
	
	// Calculate the toWorld matrix for the model
	// Apply the appropriate transformations for animations. 
	if (init) {
		model = glm::translate(model, spawn_point);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		position = spawn_point;
		init = false;
	}
	
	if (position.x >= BOUND) {
		x_move = -x_move;
	}
	if (position.y >= BOUND) {
		y_move = -y_move;
	}
	if (position.z >= BOUND) {
		z_move = -z_move;
	}
	if (position.x <= -BOUND) {
		x_move = -x_move;
	}
	if (position.y <= -BOUND) {
		y_move = -y_move;
	}
	if (position.z <= -BOUND) {
		z_move = -z_move;
	}

	glm::vec3 move = glm::normalize(glm::vec3(x_move, y_move, z_move)) * 0.003f;

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), position+move);
	glm::mat4 scale		= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 rotate	= glm::rotate(glm::mat4(1.0f), rotation+0.01f, random_vector);

	model = translate * scale * rotate;


	position = model[3];
	rotation += 0.01f;
	tick++;

	glUniformMatrix4fv(glGetUniformLocation(co2Shader, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(co2Shader, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(co2Shader, "projection"), 1, GL_FALSE, &proj[0][0]);


	currentModel.Draw(co2Shader);

}
