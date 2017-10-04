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

struct Transform
{
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 size;
	glm::mat4 objectWorldTransMat;
};

struct RigidBody
{
	float mass;
	glm::vec3 velocity;
	glm::vec3 force;
};

enum Collider{colliderless, AABB, sphere};


class Model
{
private:

	GLuint VAO;
	unsigned int vertCount;
	float t;

public:
	Model(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float mass, glm::vec3 velocity, int collider);
	~Model();

	Transform trans;
	RigidBody rb;
	Collider col;
	
	float xStart;
	float yStart;
	float zStart;

	void buffer(std::string objFile);
	void render();
	void translate();

	//physics
	void kick(glm::vec3 direction, float magnitude);
	void slide();
	void drag();
	void addForces();
	void gravity();

	//for use with collisions
	bool collidesWith(const Model &other);
	void bounce();
	void score();
	void reset();
	
};


struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd
{
	unsigned int locInd, uvInd, normInd;
};


struct Object
{
	//Transform trans;
	std::string textureFileName;
};
