#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <FreeImage.h>

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

int main() {

	//initialize the window library
	if (glfwInit() == GL_FALSE)  //initializes GLFW, returns GL_TRUE if it works
	{
		return -1;
	}

	//create a windowed mode window
	GLFWwindow* GLFWwindowPtr = glfwCreateWindow(800, 600, "Shannon Hanley DSA1 Engine", NULL, NULL);
	//creates a window with the parameters given
	//returns the address of the GLFWwindow struct
	//returns nullptr if it fails to create a window

	//make it the current active window, or quit
	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else
	{
		glfwTerminate();
		return -1;
	}

	//initialize GLEW or quit
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}


	//CHANGING THE BACKGROUND COLOR
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f); //defines the color with RGBA values

	//Model Instances
	Model floor(0, -.75, 0, 5, .25, 3, 200, glm::vec3{ 0,0,0 }, 1);
	Model ball(-2.75, -.3, 0, .08, .09, .09, 3, glm::vec3{ 0,0,0 }, 1);
	Model goal(2.75, 0, 0, .15, .7, .5, 10, glm::vec3{ 0,0,.07 }, 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	ball.buffer("models/sphere.obj");
	floor.buffer("models/box.obj");
	goal.buffer("models/box.obj");


	//SHADER HOMEWORK
	//create a shader instance
	Shader shader;

	//load the shaders
	bool shaderLoad = shader.load();

	if (shaderLoad)
	{
		//everything returned true, worked right
		shader.use();
	}


	//camera stuff
	Camera camera;

	glEnable(GL_DEPTH_TEST);

	//set lightPos
	glm::vec3 lightPos = { -4, 6, 0 };
	glUniform3f(6, lightPos.x, lightPos.y, lightPos.z);

	//texture stuff
	Texture tex;
	tex.load("textures/soccer.png");
	Texture floorTex;
	floorTex.load("textures/grass.png");
	Texture goalTex;
	goalTex.load("textures/newnet.png");

	float timer = -10;


	//Game Loop
	//loop until the user closes the window
	while (!glfwWindowShouldClose(GLFWwindowPtr)) //returns true if the window was told to close during the last frame
	{
		//Update physical simulation

		//draw buffered models

		//ACTUALLY DRAWING SOMETHING
		//clear the canvas
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render game objects

		floorTex.use();
		floor.translate();
		floor.render();
		floorTex.unbind();

		bool hit = false;
		bool hitGoal = false;

		tex.use();
		//check for kicks
		if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//update with camera direction
			timer = 100;
			ball.kick(glm::vec3{ .3,.25,0 }, 4);
		}
		timer--;
		//check collision
		hit = ball.collidesWith(floor);
		if (hit)
		{
			ball.bounce();
		}

		hitGoal = ball.collidesWith(goal);
		if (hitGoal)
		{
			ball.score();
		}

		ball.addForces();
		if (timer < 0)
		{
			ball.reset();
		}
		
		ball.translate();
		ball.render();
		tex.unbind();

		goalTex.use();
		goal.slide();
		goal.translate();
		goal.render();
		goalTex.unbind();

		//swap the front (what the screen displays) and back (what OpenGL draws to) buffers.
		glfwSwapBuffers(GLFWwindowPtr);


		camera.updateCamera();


		//process input/window events

		//close the window on esc
		if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwDestroyWindow(GLFWwindowPtr);
		}


		//Process queued window, mouse and keyboard callback events
		glfwPollEvents();  //Events queue up during a frame
	}

	glfwTerminate(); //must be called before your engine closes or you will probably leak memory or worse
	shader.unload();
	tex.unload();
	floorTex.unload();
	goalTex.unload();
}