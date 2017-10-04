#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
class Camera
{
private:
	glm::mat4 worldView;
	glm::vec3 camLoc;
	float t;

public:
	Camera();
	~Camera();

	void updateCamera();
	
};

