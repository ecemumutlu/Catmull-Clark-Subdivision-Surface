#include "Vertex.h"

Vertex::Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : 
    x(inX), y(inY), z(inZ), 
    numFaces(0), newVertexPointIndex(-1), 
    avgOfFacePoints(glm::vec3(0.0f, 0.0f, 0.0f)), 
    avgOfEdgePoints(glm::vec3(0.0f, 0.0f, 0.0f))
    { }

Vertex::Vertex(glm::vec3 inVec) :
    x(inVec.x), y(inVec.y), z(inVec.z), 
    numFaces(0), newVertexPointIndex(-1), 
    avgOfFacePoints(glm::vec3(0.0f, 0.0f, 0.0f)), 
    avgOfEdgePoints(glm::vec3(0.0f, 0.0f, 0.0f))
    { }

Vertex::~Vertex() {
    for(auto& edge: vertexEdges) {
        edge = nullptr;
    }
    vertexEdges.clear();
}

glm::vec3 Vertex::toVec3() {
    return glm::vec3(x, y, z);
}

void Vertex::addFacePointToAvg(glm::vec3& facepoint) {
    avgOfFacePoints += facepoint;
    numFaces++;
}

void Vertex::calculateAvgOfFacePoints() {
    if (numFaces == 0) {
        return;
    }
    avgOfFacePoints /= (float) numFaces;
}

void Vertex::addEdgePointsToAvg(glm::vec3& edgeMean) {
    avgOfEdgePoints += edgeMean;
}

void Vertex::calculateAvgOfEdgePoints() {
    avgOfEdgePoints /= (float) numFaces;
}

glm::vec3 Vertex::calculateNewVertexPoint() {
    glm::vec3 newVP = ( avgOfFacePoints +  avgOfEdgePoints * 2.0f + glm::vec3(x, y, z) * (float)(numFaces - 3) ) / (float) numFaces;
    return newVP;
}

void Vertex::addEdge(Edge* edge) {
    this->vertexEdges.push_back(edge);
}

void Vertex::setNewVertexPointIndex(const int& index) {
    newVertexPointIndex = index;
}

int Vertex::getNewVertexPointIndex() {
    return newVertexPointIndex;
}