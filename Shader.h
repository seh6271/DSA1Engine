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

class Shader
{
private:
	GLuint vprogram, fprogram;

	std::string filenamev = "shaders/vshader.glsl";
	std::string filenamef = "shaders/fshader.glsl";
	
	
public:
	Shader();
	~Shader();

	GLuint program;
	bool load(); //load shaders. will set value of program to 0 if it fails
	bool compile(GLenum shaderType); //helper-method used by load to read and compile a shader file and save the index where it is stored
	void use(); //calls glUseProgram(program)
	void unload(); //calls glDeleteProgram(program), glDeleteShader(vprogram), and glDeleteShader(fprogram)
					//to deallocate this shaders memory from the graphics card. set the 3 ints to 0 to indicate they are not on the graphics card
};

