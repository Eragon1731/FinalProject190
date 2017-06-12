#include "Model.h"

Model::Model(GLchar * path) {
	loadModel(path); 
}

void Model::Draw(GLint shaderProgram) {
	//Texture render
	glActiveTexture(GL_TEXTURE0); // diff 
	glUniform1i(glGetUniformLocation(shaderProgram, "cube"), 0);//diff
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); //diff

	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shaderProgram); 
	}
}

void Model::loadModel(string path) {
	//Read file via ASSIMP
		Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode * node, const aiScene * scene) {
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene.
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	//        vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates

		//            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process materials

	Mesh temp = Mesh(vertices, indices);

	if (mesh->mMaterialIndex >= 0)
	{
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN


		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		aiColor3D color(0.0f, 0.0f, 0.0f);
		//get materials
		//amb
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		temp.ambient.x = color.r;
		temp.ambient.y = color.g;
		temp.ambient.z = color.b;

		//diff
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		temp.diffuse.x = color.r;
		temp.diffuse.y = color.g;
		temp.diffuse.z = color.b;

		//spec
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		temp.specular.x = color.r;
		temp.specular.y = color.g;
		temp.specular.z = color.b;

		//shiny
		GLfloat shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);
		temp.shiny = shininess;
	}

	// Return a mesh object created from the extracted mesh data
	return temp;
}

void Model::loadTexturemap(std::vector<const GLchar*> faces)
{
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++) {

		image = loadPPM(faces[i], width, height);
		//  cerr<<"passed loadPPM: "<<endl;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	// cerr<<"outside loop"<<endl;
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//cerr<<"textureID: "<<textureID<<endl;
	//cerr<<"end of loadcubemap"<<endl;

}

unsigned char* Model::loadPPM(const char* filename, int& width, int& height) {

	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;

}
