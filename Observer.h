#pragma once
#include <Windows.h>
#include "Shader.h"
#include "Model.h"

// Std includes
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>

#include "LEAP_SDK\include\Leap.h"

class Observer {

public:
	Observer();
	~Observer();
	glm::mat4 observerWorld;


private:
	
	Model observerModel;
	GLchar* pathToObserver = "../objects/factory1/factory1.obj";
	GLchar* vertexShaderPath = "./shader_1.vert";
	GLchar* fragShaderPath = "./shader_1.frag";
	GLint observerShader;

};