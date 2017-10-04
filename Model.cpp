#include "Model.h"



Model::Model(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float mass, glm::vec3 velocity, int collider)
{
	vertCount = 0;
	VAO = 0;
	t = (float)glfwGetTime();
	
	xStart = posX;
	yStart = posY;
	zStart = posZ;

	//set collider
	if (collider == 0)
	{
		col = colliderless;
	}
	else if (collider == 1)
	{
		col = AABB;
	}
	else if (collider == 2)
	{
		col = sphere;
	}

	//set initial transforms
	
	trans.location = { posX,posY,posZ };
	glm::mat4 transMat = glm::translate(trans.location);

	trans.size = { scaleX, scaleY, scaleZ };
	glm::mat4 scaleMat = glm::scale(trans.size);

	trans.rotation = { 0,0,0 };
	glm::mat4 rotationMat = glm::yawPitchRoll(trans.rotation.x, trans.rotation.y, trans.rotation.z);

	trans.objectWorldTransMat = transMat * rotationMat * scaleMat;

	glUniformMatrix4fv(8, 1, GL_FALSE, &trans.objectWorldTransMat[0][0]);


	//set RigidBody
	rb.mass = mass;
	rb.velocity = velocity;
	rb.force = { 0,0,0 };
}


Model::~Model()
{
}

void Model::buffer(std::string objFile)
{
	//declare 4 vectors
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> vertexInds;

	//open the obj file for reading
	std::ifstream objStream;
	objStream.open(objFile);

	//read every line of the file to a string
	std::string line;
	while (std::getline(objStream, line))
	{
		//data in string stream
		std::istringstream iss(line);

		//check the label with iss
		std::string lineLabel;
		iss >> lineLabel;

		//use else if plz
		//if the label is "v"
		if (lineLabel == "v")
		{
			glm::vec3 vertexLocation;
			float x, y, z;

			iss >> x >> y >> z;

			vertexLocation = { x,y,z };

			//add to vector
			locations.push_back(vertexLocation);
		}
		//if the label is vt
		if (lineLabel == "vt")
		{
			float x, y;
			glm::vec2 uvLocation;

			iss >> x >> y;

			uvLocation = { x,y };

			//add to vector
			uvs.push_back(uvLocation);
		}
		//if the label is vn
		if (lineLabel == "vn")
		{
			glm::vec3 normalLocation;
			float x, y, z;

			iss >> x >> y >> z;

			normalLocation = { x,y,z };

			//add to vector
			normals.push_back(normalLocation);
		}
		//if the label is f
		if (lineLabel == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				unsigned int a, b, c;
				char slash;

				iss >> a >> slash >> b >> slash >> c;

				a--;
				b--;
				c--;

				VertInd fs = { a,b,c };

				vertexInds.push_back(fs);
			}
		}

	}
	//string is over 
	objStream.close();	
	
	vertCount = vertexInds.size();
	
	//processing model data
	//Duplicate vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locations[vertexInds[i].locInd], uvs[vertexInds[i].uvInd], normals[vertexInds[i].normInd] };
	}

	//Step 1: Generating a buffer (VBO)
	//GLuint VAO;
	GLuint VBO;

	glGenVertexArrays(1, &VAO);  //openGL with create the buffers internally and give us back the indices
	glGenBuffers(1, &VBO);

	//Step 2: Bind the Buffer
	//call glBindBuffer to control which array and buffer are active

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //takes a "target" (GL_ARRAY_BUFFER) which holds the raw vertex data


	//Step 3: Storing Data in the Buffer
	//copies specified data into currently bound buffer
	glBufferData(GL_ARRAY_BUFFER,	//where to copy to
		sizeof(Vertex) * vertCount,		//# of bytes to copy
		&vertBufData[0],		//Where to copy from
		GL_STATIC_DRAW);		//"hint" to OpenGL


	//Step 4: Describe the Buffer Layout
	//enable the attribute
	glEnableVertexAttribArray(0);	//Attribute index - 0

	//set up the attribute
	glVertexAttribPointer(
		0,				//Attribute index - 0 in this case
		3,				//Number of components (x, y, z)
		GL_FLOAT,		//Type of Data
		GL_FALSE,		//Should we normalize the data? (0-1)
		sizeof(Vertex),	//Stride (how many bites in this attribute?)
		0);					//Offset (where does this attribute start?)


	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*) sizeof(glm::vec3));

	glEnableVertexAttribArray(2);

	unsigned int offsetSize = sizeof(glm::vec3) + sizeof(glm::vec2);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*) offsetSize);
	
	glBindVertexArray(0);	//unbind when finished editing

	
	return;
}

void Model::render()
{
	//render game objects
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
	glBindVertexArray(0);	//unbind after drawing
}

void Model::translate()
{
	//physics
	//time
	float t0 = t;
	t = (float)glfwGetTime();
	float dt = t - t0;



	
	glm::mat4 transMat = glm::translate(trans.location);

	
	glm::mat4 scaleMat = glm::scale(trans.size);

	
	glm::mat4 rotationMat = glm::yawPitchRoll(trans.rotation.x, trans.rotation.y, trans.rotation.z);

	trans.objectWorldTransMat = transMat * rotationMat * scaleMat;

	glUniformMatrix4fv(8, 1, GL_FALSE, &trans.objectWorldTransMat[0][0]);


}

//create a force in the direction of the kick
void Model::kick(glm::vec3 direction, float magnitude)
{
	//direction + magnitude = force of kick
	rb.force = direction * magnitude;

	//create initial speed of ball
	rb.velocity = rb.force / rb.mass;
}


//move goalposts back and forth
void Model::slide()
{
	//move goalposts back and forth
	float maxZ = 2;

	if (trans.location.z > maxZ || trans.location.z < maxZ * -1)
	{
		//flip direction
		rb.velocity.z *= -1;
	}

	trans.location += rb.velocity;

}

//slowly lower velocity through air resistance (like friction)
void Model::drag()
{
	//create the drag acceleration
	glm::vec3 airResistance = rb.velocity;

	//accel is in the opposite direction and smaller
	airResistance *= -1;
	airResistance *= .05;
	
	//add to velocity
	rb.velocity += airResistance;
}

void Model::addForces()
{
	//add drag
	drag();

	//add gravity
	gravity(); 

	//add velocity to movement
	trans.location += rb.velocity;

}

void Model::gravity()
{
	//if above the ground
	if (trans.location.y > -.25)
	{
		//add a downward force
		glm::vec3 grav = { 0,0,0 };

		float gravity = rb.mass * -.01;

		grav.y = gravity;
		
		rb.velocity += grav;
	}

	//fake collision
	/*if (trans.location.y < -.4)
	{
		trans.location.y = -.4;
	}*/
}

//returns false if the objects are seperated
bool Model::collidesWith(const Model & other)
{
	//test 1: is either object colliderless?
	if (col == colliderless || other.col == colliderless)
	{
		//there are no collisions here
		return false;
	}

	//test 2: if both are spheres
	if (col == sphere && other.col == sphere)
	{
		//if the distance between the objects' centers is greater than the sum of their radii, they are touching
		
		//step 1: calculate the distance
		//(z1 - z2)^2 + (x1 - x2)^2 + (y1 - y2)^2 = distance^2
		float distance = pow((trans.location.z - other.trans.location.z), 2) + pow((trans.location.x - other.trans.location.x), 2) + pow((trans.location.y - other.trans.location.y), 2);
		distance = sqrt(distance);

		//step 2: sum of the radii
		//radii are size.x 
		float sum = trans.size.x + other.trans.size.x;

		//test for collision
		if (distance > sum)
		{
			//no collision
			return false;
		}
	}

	//test 3: if both are AABB
	if (col == AABB && other.col == AABB)
	{
		//check all three axises for overlap, if one does not there is no collision

		//calculate variables needed
		float disX = pow(trans.location.x - other.trans.location.x, 2);
		disX = sqrt(disX);
		float disY = pow(trans.location.y - other.trans.location.y, 2);
		disY = sqrt(disY);
		float disZ = pow(trans.location.z - other.trans.location.z, 2);
		disZ = sqrt(disZ);

		float sumX = trans.size.x + other.trans.size.x;
		float sumY = trans.size.y + other.trans.size.y;
		float sumZ = trans.size.z + other.trans.size.z;

		//does the x axis overlap
		if (disX > sumX)
		{
			//no collide
			return false;
		}
		if (disY > sumY)
		{
			//no collide
			return false;
		}
		if (disZ > sumZ)
		{
			//no collide
			return false;
		}

		
	}

	//test 4: one is a sphere and the other is AABB
	if ((col == AABB && other.col == sphere) || (col == sphere && other.col == AABB))
	{
		
		//calculate the needed variables
		float minX = other.trans.location.x - other.trans.size.x / 2;
		float minY = other.trans.location.y - other.trans.size.y / 2;
		float minZ = other.trans.location.z - other.trans.size.z / 2;

		float maxX = other.trans.location.x + other.trans.size.x / 2;
		float maxY = other.trans.location.y + other.trans.size.y / 2;
		float maxZ = other.trans.location.z + other.trans.size.z / 2;

		float distance = 0;

		//check if center of sphere on each axis falls between min and max
		//x check
		if (trans.location.x < minX)
		{
			distance += pow((minX - trans.location.x),2);
		}
		else if (trans.location.x > maxX)
		{
			distance += pow((trans.location.x - maxX),2);
		}
		//y check
		if (trans.location.y < minY)
		{
			distance += pow((minY - trans.location.y), 2);
		}
		else if (trans.location.y > maxY)
		{
			distance += pow((trans.location.y - maxY),2);
		}
		//z check
		if (trans.location.x < minX)
		{
			distance += pow((minX - trans.location.x),2);
		}
		else if (trans.location.x > maxX)
		{
			distance += pow((trans.location.x - maxX),2);
		}

		//if distance is less than the radius of the sphere, they are colliding
		if (distance < pow(trans.size.x / 2,2))
		{
			//collide
			return false;
		}
	}

	//none of the above falses were triggered so there must have been a collsion
	return true;
}

void Model::bounce()
{
	//temp function, just reverses y direction for a simple bounce
	//will use normal force in future
	rb.velocity.y *= -1;
}

void Model::score()
{
	//slow ball down
	rb.velocity *= .9;

	//change direction
	rb.velocity.x *= -1;
}

void Model::reset()
{
	trans.location = { xStart,yStart, zStart };
	rb.velocity *= 0;
}

