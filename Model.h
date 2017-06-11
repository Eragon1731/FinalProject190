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

class Model {

public:
	Model(GLchar * path);
	~Model(); 
	void Draw(GLint shaderProgram); 

private:
	std::vector<Mesh> meshes;
	std::string directory; 
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path); 
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char *path, const std::string &directory);
};