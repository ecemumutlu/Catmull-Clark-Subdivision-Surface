#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include "GL/glew.h"
#include <vector>


struct Texture
{
	Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
	GLfloat u, v;
};

class Normal
{
	public:
	Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
	Normal(glm::vec3 inVec) : x(inVec.x), y(inVec.y), z(inVec.z) { }
	GLfloat x, y, z;
};


#endif


