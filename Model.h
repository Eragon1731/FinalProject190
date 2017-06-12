#pragma once
#include <Windows.h>
#include "shader.h"
#include "Mesh.h"

#include <stdio.h>
#include <vector>



#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc\matrix_transform.hpp>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

using namespace std; 

class Model {

public:
	Model() {}
	Model(GLchar * path);

	void Draw(GLint shaderProgram); 
	void loadTexturemap(std::vector<const GLchar*> faces);
	GLuint textureID;
private:
	vector<Mesh> meshes;
	string directory; 

	void loadModel(string path); 
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};