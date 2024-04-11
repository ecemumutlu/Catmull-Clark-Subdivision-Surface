#ifndef FACE_H
#define FACE_H

#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <math.h>
#include <cmath>
#include <iostream>
#include "Structs.h"
#include "Vertex.h"
#include <vector>
#include <vector>
class Edge;
using namespace std;
class Face
{
	public:
	//GLuint vIndex[3], tIndex[3], nIndex[3];
	std::vector<GLuint> vIndex, nIndex;
    int size;
    std::vector<Edge*> faceEdges;
	int facePointIndex;
	glm::vec3 faceNormal; // flat shading


	Face(int v[], int size) : size(size) , facePointIndex(-1) {
		for (int i = 0; i < size ; i++)
		{
			vIndex.push_back(v[i]);
			// nIndex.push_back(n[i]);
		}
	}

	Face(GLuint v[],  int size, glm::vec3 normal) : size(size) , facePointIndex(-1), faceNormal(normal){
		for (int i = 0; i < size ; i++)
		{
			vIndex.push_back(v[i]);
			//nIndex.push_back(n[i]);

		}
	}

	Face(vector<int> v,  int size) : size(size) , facePointIndex(-1) {
		for (int i = 0; i < size ; i++)
		{
			vIndex.push_back(v[i]);
			// nIndex.push_back(n[i]);

		}
	}

	Face(vector<int>& v, int size, glm::vec3 fNormal) : size(size) , facePointIndex(-1), faceNormal(fNormal) {
		for (int i = 0; i < size ; i++)
		{
			vIndex.push_back(v[i]);
			// nIndex.push_back(0); //todo: calculate normals here
		}
	}

	
	~Face() {
		faceEdges.clear();
	}

    glm::vec3 calculateFacePoint(std::vector<Vertex>& vertices);
    int getSize();

	int getFacePointIndex();
	void setFacePointIndex(int index); 
	void addEdge(Edge* edge);
	void setFaceNormal(glm::vec3 normal);
	glm::vec3 getFaceNormal();
	void setVIndex(int a1, int a2, int a3);

};

#endif