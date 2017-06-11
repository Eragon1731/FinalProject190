#pragma once

//std includes
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <math.h>




#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc\matrix_transform.hpp>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;

};

class Mesh {

public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector <Texture> textures; 
	Mesh() {}
	Mesh(std::vector <Vertex> vertices, std::vector <GLuint> indices, std::vector <Texture> textures = std::vector<Texture>());
	void draw(GLint shader);

	glm::vec3 ambient, diffuse, specular;
	GLfloat shiny; 

	GLuint uProjection, uView, uModel;


private:
	GLuint VAO, VBO, EBO;
	void setupMesh(); 
};