#pragma once
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "rpc\client.h"
//#include "Cube.h"
#include "shader.h"
#include "StereoCamera.h"
#include "LEAP_SDK\include\Leap.h"
#include "LEAP_SDK\include\LeapMath.h"

using namespace Leap; 
class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void resetGame(); 
	static void renderMolecules(glm::mat4 projection, glm::mat4 view); 
	static void checkMoleculeIntersection(); 
	static float intersection(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 moleculePos, float radius);
};

#endif
