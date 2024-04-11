#include "Program.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
//#include <OpenGL/gl3.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 


bool Program::ReadDataFromFile(
	const string& fileName, ///< [in]  Name of the shader file
	string& data)     ///< [out] The contents of the file
{
	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);

	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			data += curLine;
			if (!myfile.eof())
			{
				data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		return false;
	}

	return true;
}

GLuint Program::createVS(const string& shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &shader, &length);
	glCompileShader(vs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(vs, 1024, &length, output);
	//printf("VS compile log: %s\n", output);

	return vs;
}

GLuint Program::createFS(const string& shaderName)
{
	string shaderSource;

	string filename(shaderName);
	if (!ReadDataFromFile(filename, shaderSource))
	{
		cout << "Cannot find file name: " + filename << endl;
		exit(-1);
	}

	GLint length = shaderSource.length();
	const GLchar* shader = (const GLchar*)shaderSource.c_str();

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &shader, &length);
	glCompileShader(fs);

	char output[1024] = { 0 };
	glGetShaderInfoLog(fs, 1024, &length, output);
	//printf("FS compile log: %s\n", output);

	return fs;
}


void Program::initShaders(const string& vertexShader, const string& fragmentShader) {

	// Create the programs
	this->gProgram = glCreateProgram();

	// Create the shaders for both programs
	this->vs = createVS(vertexShader);
	this->fs_normal = createFS(fragmentShader);

	// Attach the shaders to the programs
	glAttachShader(this->gProgram, this->vs);
	glAttachShader(this->gProgram, this->fs_normal);

	// Link the programs
	glLinkProgram(this->gProgram);
	GLint status;
	glGetProgramiv(this->gProgram, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		std::cout << "Program link failed, init shaders error" << std::endl;
		exit(-1);
	}
}

void Program::useProgram() {
	glUseProgram(this->gProgram);
}