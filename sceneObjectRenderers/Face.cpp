#include "Face.h"


glm::vec3 Face::calculateFacePoint(std::vector<Vertex>& vertices) {
    glm::vec3 fp = glm::vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < this->size; ++i) {
        fp += vertices[this->vIndex[i]].toVec3();
    }
    fp /= (float) this->size;

    return fp;
}

int Face::getSize() {
    return this->size;
}


int Face::getFacePointIndex() {
    return this->facePointIndex;
}

void Face::setFacePointIndex(int index) {
    this->facePointIndex = index;
}

void Face::addEdge(Edge* edge) {
    this->faceEdges.push_back(edge);
}

void Face::setFaceNormal(glm::vec3 normal) {
    this->faceNormal = normal;
}

glm::vec3 Face::getFaceNormal() {
    return this->faceNormal;
}

void Face::setVIndex(int a1, int a2, int a3) {
    this->vIndex.clear();
    this->vIndex.push_back(a1);
    this->vIndex.push_back(a2);
    this->vIndex.push_back(a3);
}



