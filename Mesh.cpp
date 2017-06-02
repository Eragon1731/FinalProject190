#include "Mesh.h"

Mesh::Mesh(vector <Vertex> vertices, vector <GLuint> indices){
	//, vector <Texture> textures){

	this->vertices = vertices;
	this->indices = indices;
//	this->textures = textures;

	setupMesh(); 

}

void Mesh::setupMesh() {

	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	//         Vertex Texture Coords
	//        glEnableVertexAttribArray(2);
	//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glBindVertexArray(0);

}

void Mesh::draw(GLint shader) {

	//lights:directional
	glUniform3f(glGetUniformLocation(shader, "direction"), 1.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shader, "ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(shader, "diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader, "specular"), 0.5f, 0.5f, 0.5f);

	////materails
	glUniform3f(glGetUniformLocation(shader, "amb"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shader, "diff"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shader, "spec"), specular.x, specular.y, specular.z);
	glUniform1f(glGetUniformLocation(shader, "shininess"), shiny);


	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(this->VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}