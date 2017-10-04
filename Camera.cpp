#include "Camera.h"



Camera::Camera()
{
	camLoc = { -4.5,.75, 2 };
	t = (float)glfwGetTime();
}


Camera::~Camera()
{
}

void Camera::updateCamera()
{
	//LookAt Mat	
	glm::vec3 camRot = { 0,0,0 };

	//turn with mouse
	float sens = .005;
	int w = 800;
	int h = 600;
	double x;
	double y;
	glfwGetCursorPos(glfwGetCurrentContext(),&x, &y);

	camRot.y -= sens * (x - w * .5f);
	camRot.x -= sens * (y - h * .5f);
	camRot.x = glm::clamp(camRot.x, -.5f * 3.14159f, .5f * 3.14159f);

	//glfwSetCursorPos(glfwGetCurrentContext(), w * .5f, h * .5f);
	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	//time change
	float t0 = t;
	t = (float)glfwGetTime();
	float dt = t - t0;


	glm::vec3 camVel;

	//move with arrows
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camVel += rotMat * glm::vec3(-1, 0, 0);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camVel += rotMat * glm::vec3(1, 0, 0);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camVel += rotMat * glm::vec3(0, 0, 1);
	}
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camVel += rotMat * glm::vec3(0, 0, -1);
	}

	float speed = 1.f;
	if (camVel != glm::vec3())
	{
		camVel = glm::normalize(camVel) * speed;
	}
	camLoc += camVel * dt;




	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);



	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);

	//Perspective Mat
	float zoom = 1.f;
	int width = 800;
	int height = 600;

	float fovy = 3.14159 * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	glm::mat4 perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);

	//new cam matrix
	glm::mat4 camMat = perspectiveMat * lookAtMat;


	glUniformMatrix4fv(5, 1, GL_FALSE, &camMat[0][0]);
	glUniform3f(7, camLoc.x, camLoc.y, camLoc.z);
}


