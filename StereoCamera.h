#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>

class StereoCamera {

public:
	StereoCamera(
		float Convergence,
		float EyeSeparation,
		float AspectRatio,
		float FOV,
		float Near,
		float Far
	);
	~StereoCamera();
	glm::mat4 ApplyLeftFrustum();
	glm::mat4 ApplyRightFrustum();

private:
	float mConvergence;
	float mEyeSeparation;
	float mAspectRatio;
	float mFOV;
	float mNear;
	float mFar;

};