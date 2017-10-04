#include "Shader.h"



Shader::Shader()
{
}


Shader::~Shader()
{
}

bool Shader::load()
{
	//call compile() for both vertex and frag
	bool vertTest = compile(GL_VERTEX_SHADER);
	bool fragTest = compile(GL_FRAGMENT_SHADER);

	//if either failed, there was an error
	if (!vertTest || !fragTest)
	{
		//test failed
		return false;
	}

	//create a shader program
	program = glCreateProgram();

	//attach shaders
	glAttachShader(program, vprogram);
	glAttachShader(program, fprogram);

	//link program
	glLinkProgram(program);


	//was the program correctly linked?
	//test if compiled
	GLint ltest;
	glGetProgramiv(program, GL_LINK_STATUS, &ltest);
	if (ltest != 0)
	{
		//compiled successfully
		return true;
	}
	else
	{
		//did not compile, get info			
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char* log = new char[length];

		glGetProgramInfoLog(program, length, 0, log);
		std::cout << "ERROR COMPILING PROGRAM: " << log << std::endl;

		//delete extras
		glDeleteProgram(program);
		delete log;

		//failure
		return false;
	}



	return false;
}

bool Shader::compile(GLenum shaderType)
{
	//for vertex shader
	if (shaderType == GL_VERTEX_SHADER)
	{
		//a.
		//open the shader file, load it into an array
		std::ifstream vertStream;
		vertStream.open(filenamev);
		char* vertString;
		if (vertStream.is_open()) {
			//Length
			vertStream.seekg(0, std::ios::end); //Go to the end of the file
			int length = (int)vertStream.tellg(); // How many bytes into the file are we?
			vertStream.seekg(0, std::ios::beg); //Go to the beginning of the file

			//Read & Close
			vertString = new char[length + 1]; //extra byte for null terminator
			vertString[length - 21] = 0; //fix the null pointer
			vertStream.read(vertString, length);
			vertStream.close();
		}
		else
		{
			//problem opening
			std::cout << "ERROR OPENING VERTEX FILE" << std::endl;
			return false;
		}
		//b.
		//create vertex shader
		vprogram = glCreateShader(shaderType);
		//set source code
		glShaderSource(vprogram, 1, &vertString, 0);
		//Compile shader
		glCompileShader(vprogram);
		//delete array
		delete vertString;

		//c.
		//test if compiled
		GLint vtest;
		glGetShaderiv(vprogram, GL_COMPILE_STATUS, &vtest);

		if (vtest != 0)
		{
			//compiled successfully
			return true;
		}
		else
		{
			//did not compile, get info			
			GLint length;
			glGetShaderiv(vprogram, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];

			glGetShaderInfoLog(vprogram, length, 0, log);
			std::cout << "ERROR COMPILING VERTEX: " << log << std::endl;

			//delete extras
			glDeleteShader(vprogram);
			delete log;

			//failure
			return false;
		}
	}
	//end vertex shader if statement
	if (shaderType == GL_FRAGMENT_SHADER)
	{
		//a.
		//open the shader file, load it into an array
		std::ifstream fragStream;
		fragStream.open(filenamef);
		char* fragString;
		if (fragStream.is_open()) {
			//Length
			fragStream.seekg(0, std::ios::end); //Go to the end of the file
			int length = (int)fragStream.tellg(); // How many bytes into the file are we?
			fragStream.seekg(0, std::ios::beg); //Go to the beginning of the file

			//Read & Close
			fragString = new char[length + 1]; //extra byte for null terminator
			fragString[length - 35] = 0; //fix the null pointer
			fragStream.read(fragString, length);
			fragStream.close();
		}
		else
		{
			//problem opening
			std::cout << "ERROR OPENING FRAGMENT FILE" << std::endl;
			return false;
		}
		//b.
		//create vertex shader
		fprogram = glCreateShader(shaderType);
		//set source code
		glShaderSource(fprogram, 1, &fragString, 0);
		//Compile shader
		glCompileShader(fprogram);
		//delete array
		delete fragString;

		//c.
		//test if compiled
		GLint ftest;
		glGetShaderiv(fprogram, GL_COMPILE_STATUS, &ftest);

		if (ftest != 0)
		{
			//compiled successfully
			return true;
		}
		else
		{
			//did not compile, get info			
			GLint length;
			glGetShaderiv(fprogram, GL_INFO_LOG_LENGTH, &length);

			char* log = new char[length];

			glGetShaderInfoLog(fprogram, length, 0, log);
			std::cout << "ERROR COMPILING FRAGMENT: " << log << std::endl;

			//delete extras
			glDeleteShader(vprogram);
			delete log;

			//failure
			return false;
		}
	}
	//end of fragment if statement

	return false;
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);
	program = 0;
	vprogram = 0;
	fprogram = 0;
}
