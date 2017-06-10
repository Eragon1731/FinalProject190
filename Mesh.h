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

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	GLuint id;
	string type;
	aiString path;

};

class Mesh {

public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector <Texture> textures; 

	Mesh(vector <Vertex> vertices, vector <GLuint> indices);//, vector <Texture> textures);
	void draw(GLint shader, GLuint textureID);

	glm::vec3 ambient, diffuse, specular;
	GLfloat shiny; 

	GLuint uProjection, uView, uModel;


private:
	GLuint VAO, VBO, EBO;
	void setupMesh(); 
};