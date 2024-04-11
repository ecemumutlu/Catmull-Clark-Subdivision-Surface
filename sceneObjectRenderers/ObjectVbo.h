#pragma once
#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "Structs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include "Face.h"
#include "Edge.h"
#include "Vao.h"
#include "Vbo.h"
#include "Ebo.h"

using namespace std;

class ObjectVbo {
public:
    vector<Vertex> oVertices;
	// std::vector<Texture> oTextures;
	vector<Normal> oNormals;
	vector<Face> oFaces; // triangle faces
	vector<Face> oQuadFaces; // quad faces

	vector<glm::vec3> baseVertices; // will be used for decrease level
	vector<Face> baseFaces; // will be used for decrease level

	vector<Vao> vao; // va0:0 -> solid, vao:1 -> line and wire
	vector<Ebo> ebo;
	vector<Vbo> vbo; // va0:0 -> solid, vao:1 -> line and wire

	int edgeDataSize;
	int mode; // mode:0 -> solid, mode:1 -> wire, mode:2 -> line

	// Constructor
	ObjectVbo()
	: mode(0), edgeDataSize(0) //, gFrameBuffer(0), gDepthBuffer(0)
	{ 
		vao.push_back(Vao()); vao.push_back(Vao()); 
		ebo.push_back(Ebo()); ebo.push_back(Ebo());
		vbo.push_back(Vbo()); vbo.push_back(Vbo()); 
	}


	~ObjectVbo();

	int getVertexDataSize();
	int getNormalDataSize();
	int getIndexDataSize();

	void initVBO();
	void parseObjectFromFile(const string& fileName, glm::vec3 color);

	void triangulateQuadFace();
	void calculateNormals();
	vector<Face> getFaces();
	vector<Vertex>& getVertices();
	int getFaceSize();	
	vector<Face>& getQuadFaces();
	int getQuadFaceSize();
	void returnBaseVertices();
	void initWireframeVBO();

    void replaceVertices(vector<glm::vec3>& calculatedVertices);
};