#include "StereoCamera.h"

StereoCamera::StereoCamera(float Convergence,
	float EyeSeparation,
	float AspectRatio,
	float FOV,
	float Near,
	float Far) {


	mConvergence = Convergence;
	mEyeSeparation = EyeSeparation;
	mAspectRatio = AspectRatio;
	mFOV = FOV;
	mNear = Near;
	mFar = Far;
}

glm::mat4 StereoCamera::ApplyLeftFrustum() {

	float top, bottom, left, right;

	top = mNear * tan(mFOV / 2.0f);
	bottom = -top;

	float a = mAspectRatio * tan(mFOV / 2.0f) * mConvergence;

	float b = a - mEyeSeparation / 2.0f;
	float c = a + mEyeSeparation / 2.0f;

	left = -b * (mNear / mConvergence);
	right = c * (mNear / mConvergence);

	//set projection matrix

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::frustum(left, right, bottom, top, mNear, mFar);

	projection = projection * glm::translate(glm::mat4(1.0f), glm::vec3(mEyeSeparation / 2.0f, 0.0f, 0.0f));

	return projection;
}

glm::mat4 StereoCamera::ApplyRightFrustum() {

	float top, bottom, left, right;

	top = mNear * tan(mFOV / 2.0f);
	bottom = -top;

	float a = mAspectRatio * tan(mFOV / 2.0f) * mConvergence;

	float b = a - mEyeSeparation / 2.0f;
	float c = a + mEyeSeparation / 2.0f;

	left = -c * (mNear / mConvergence);
	right = b * (mNear / mConvergence);

	//set projection matrix

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::frustum(left, right, bottom, top, mNear, mFar);

	projection = projection * glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f * (mEyeSeparation / 2.0f), 0.0f, 0.0f));

	return projection;
}