#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <algorithm>
#include "Structs.h"
#include "Face.h"
#include "Vertex.h"


class Edge {

public:
    std::vector<Vertex*> edgeVertices;
    std::vector<Face*> edgeFaces;
    int v1;
    int v2;
    int edgePointIndex; 

    Edge(int v1, int v2) : v1(std::min(v1, v2)), v2(std::max(v1, v2)), edgePointIndex(-1) {}

    ~Edge() {
        for (auto& vertex : edgeVertices) {
            vertex = nullptr;
        }
        edgeVertices.clear();

        for (auto& face : edgeFaces) {
            face = nullptr;
        }
        edgeFaces.clear();
    }

    bool operator==(const Edge& other) const {
        return (v1 == other.v1 && v2 == other.v2);
    }

    void addEdgeMeanToVertices() {
        glm::vec3 mean = ( edgeVertices[0]->toVec3() + edgeVertices[1]->toVec3() ) / 2.0f;

        edgeVertices[0]->addEdgePointsToAvg(mean);
        edgeVertices[1]->addEdgePointsToAvg(mean);
    }

    int getFirstVertexIndex() {
        return v1;
    }

    int getSecondVertexIndex () {
        return v2;
    }

    void addFace(Face* face) {
        this->edgeFaces.push_back(face);
    }

    void addVertex(Vertex* vertex) {
        this->edgeVertices.push_back(vertex);
    }

    void setEdgePointIndex(int index) {
        this->edgePointIndex = index;
    }


};

// Hash function for unordered_map
struct EdgeHash {
    std::size_t operator()(const Edge& edge) const {
        return std::hash<int>()(edge.v1) ^ std::hash<int>()(edge.v2);
    }
};



#endif