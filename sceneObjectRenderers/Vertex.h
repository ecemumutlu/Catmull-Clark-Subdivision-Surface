
#ifndef VERTEX_H
#define VERTEX_H
#include "GL/glew.h"
#include <vector>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <math.h>
#include <cmath>
#include <iostream>

class Edge;

class Vertex
{
	public:
	GLfloat x, y, z;
	glm::vec3 avgOfFacePoints;
	glm::vec3 avgOfEdgePoints;
	int numFaces;
	std::vector<Edge*> vertexEdges;
	int newVertexPointIndex;

	Vertex(GLfloat inX, GLfloat inY, GLfloat inZ);

	Vertex(glm::vec3 inVec);

	~Vertex() ;

    void setNewVertexPointIndex(const int& index);
	int getNewVertexPointIndex();
    glm::vec3 toVec3() ;

	void calculateAvgOfFacePoints();
	void calculateAvgOfEdgePoints();
	glm::vec3 calculateNewVertexPoint();

	void addEdge(Edge* edge);
    void addFacePointToAvg(glm::vec3& facepoint);
    void addEdgePointsToAvg(glm::vec3& edgeMean);
};

#endif