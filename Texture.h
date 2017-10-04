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
#include <sstream>
#include <iostream>

class Texture
{
private:
	unsigned int texID;
public:
	Texture();
	~Texture();
	void load(char* texFile);
	void unload();
	void use();
	void unbind();
};

