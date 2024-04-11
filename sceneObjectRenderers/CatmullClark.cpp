#include "CatmullClark.h"
#include <set>

CatmullClark::CatmullClark(Object& obj, int baseLevel)
    : object(obj), level(0), baseLevel(baseLevel) {
        this->object.triangulate();
		this->object.initBuffers(); // initVBO of solid, initVBO of wireframe

        for(int i = 0; i < baseLevel; i++) {
            increaseLevel();
        }
     }

CatmullClark::~CatmullClark() {
    for (auto& edgePtr : this->edges) {
        delete edgePtr;
    }
    this->edges.clear();
    this->edgePointsCoord.clear();
    this->facePointsCoord.clear();
    this->newVertexPointsCoord.clear();
    this->catmullFaces.clear();
    this->newEdgesForWireframe.clear();
}

void CatmullClark::increaseLevel() {
    initFaceAndEdgePoints();
    this->object.triangulate();
    this->object.replaceVertices(newVertexPointsCoord);
    this->object.initBuffers(); // initVBO of solid, initVBO of wireframe
    this->level += 1;
}

void CatmullClark::changeMode() {
    this->object.changeMode();
}


void CatmullClark::decreaseLevel() {
	if (this->level > 0) {
        int tmpLevel = this->level - 1;
		this->level = 0;
        this->object.returnBaseVertices(); // set oFaces and oVertices to baseFaces and baseVertices
        this->object.initBuffers(); // initVBO of solid, initVBO of wireframe

        for(int i = 0; i < tmpLevel; i++) {
            increaseLevel();
        }
	}
}

void CatmullClark::reset() {
    this->object.resetMatrixVariables();
    if (this->level == baseLevel) return;

    this->object.returnBaseVertices();
    this->object.initBuffers(); // initVBO of solid, initVBO of wireframe
    this->level = 0;

    for (int i = 0; i < baseLevel; i++) {
        increaseLevel();
    }
}

void CatmullClark::start_pause() {
    this->object.start_pause();
}

void CatmullClark::setMatrices(double deltaTime) {
    object.setMatrices(deltaTime);
}

void CatmullClark::setProjectionMatrix(glm::mat4 proj) {
    object.setProjectionMatrix(proj);
}

void CatmullClark::setViewingMatrix(glm::mat4 view) {
    object.setViewingMatrix(view);
}

void CatmullClark::drawObject() {
    
    object.drawObject();
}

void CatmullClark::setDepthBuffer() {
    object.setDepthBuffer();
}


void CatmullClark::initFaceAndEdgePoints() {
    edgePointsCoord.clear();
    facePointsCoord.clear();
    newVertexPointsCoord.clear();
    catmullFaces.clear();
    newEdgesForWireframe.clear();


    vector<Face>& faces = object.objectVbo.getQuadFaces();
    vector<Vertex>& vertices = object.objectVbo.getVertices();


    std::unordered_map<Edge, std::vector<int>, EdgeHash> edgeToFaceMap = findFacesForEachEdge(faces);

    // Calculate face points
    for (int i = 0; i < faces.size(); ++i) {
        Face& face = faces[i];

        glm::vec3 fpCoord = face.calculateFacePoint(vertices);
        this->facePointsCoord.push_back(fpCoord);
        face.setFacePointIndex(i);

        // Calculate average of face points contributing to their corresponding vertex
        for(auto& index: face.vIndex) {
            Vertex& vertex = vertices[index];
            vertex.addFacePointToAvg(fpCoord);
        }
    }

    for (auto& vertex: vertices) {
        vertex.calculateAvgOfFacePoints();
    }


    // Calculate edge points
    int i = 0;
    for (auto& entry : edgeToFaceMap) {
        Edge tmpEdge = entry.first;
        Edge* edge = new Edge(tmpEdge.getFirstVertexIndex(), tmpEdge.getSecondVertexIndex());
        std::vector<int>& faceIndices = entry.second;
        
        Face& face1 = faces[faceIndices[0]];
        Face& face2 = faces[faceIndices[1]];


        // Calculate edge point (F1 + F2 + V1 + V2) / 4.0f 
        glm::vec3 sum = this->facePointsCoord[face1.getFacePointIndex()] + 
                            this->facePointsCoord[face2.getFacePointIndex()] + 
                            vertices[edge->v1].toVec3() + 
                            vertices[edge->v2].toVec3();

        // Store results in the edgePointsCoord vector
        this->edgePointsCoord.push_back(sum / 4.0f);

        // Create an EdgePoint object and store it in the edgePoints vector
        // EdgePoint object holds the index of the edge point in the edgePointsCoord vector
        edge->setEdgePointIndex(i);

        edge->addFace(&face1);
        edge->addFace(&face2);

        edge->addVertex(&vertices[edge->v1]);
        edge->addVertex(&vertices[edge->v2]);

        face1.addEdge(edge);
        face2.addEdge(edge);

        vertices[edge->v1].addEdge(edge);
        vertices[edge->v2].addEdge(edge);

        this->edges.push_back(edge); 
        edge = nullptr;
        i++;       
    }

    // Calculate average of edge points contributing to their corresponding vertex
    for (auto& edge: this->edges) {
        edge->addEdgeMeanToVertices();
    }

    for (auto& vertex: vertices) {
        vertex.calculateAvgOfEdgePoints();
    }

    // Calculate new vertex points

    for (auto& vertex: vertices) {
        glm::vec3 newVP = vertex.calculateNewVertexPoint();
        vertex.setNewVertexPointIndex(this->newVertexPointsCoord.size());
        this->newVertexPointsCoord.push_back(newVP);
    }

    // merge all new vertices into one vector
    int sizeFacePoints = this->facePointsCoord.size();
    int sizeEdgePoints = this->edgePointsCoord.size();
    int sizeNewVertexPoints = this->newVertexPointsCoord.size();


    this->newVertexPointsCoord.insert(this->newVertexPointsCoord.end(), this->facePointsCoord.begin(), this->facePointsCoord.end());
    this->newVertexPointsCoord.insert(this->newVertexPointsCoord.end(), this->edgePointsCoord.begin(), this->edgePointsCoord.end());

    int paddingFP = sizeNewVertexPoints;
    int paddingEP = sizeNewVertexPoints + sizeFacePoints;
 
    // Create Faces
    int j = 0;
    for (auto& vertex: vertices) {
        int nv_index = vertex.getNewVertexPointIndex();
    
        for (auto& edge: vertex.vertexEdges) {
            vector<int> faceIndices;
            int ep_index = edge->edgePointIndex + paddingEP ;
            faceIndices.push_back(nv_index);
            faceIndices.push_back(ep_index);
            glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
            for (auto& face: edge->edgeFaces) {
                // normal = (fp - r)  x (p_new - r) 
                // if normal . view_direction < 0 -> frontfacing
                // eye is at glm::vec3(0,0,0)
                int fp_index = face->getFacePointIndex() + paddingFP ;
                normal = glm::normalize( 
                                    glm::cross(                                         
                                        glm::normalize(newVertexPointsCoord[fp_index] - newVertexPointsCoord[ep_index]), 
                                        glm::normalize(newVertexPointsCoord[nv_index] - newVertexPointsCoord[ep_index])         
                                    )
                                );
                glm::vec3 viewDirection = glm::normalize(newVertexPointsCoord[ep_index] - glm::vec3(0,0,0));
                
                if (glm::dot(normal, viewDirection) > 0) {
                    faceIndices.push_back(fp_index);
                    face->setFaceNormal(normal);

                    for (auto& edgeFace: face->faceEdges) {
                        if ( edgeFace->edgePointIndex != edge->edgePointIndex && (edgeFace->v1 == nv_index || edgeFace->v2 == nv_index) ) {
                            faceIndices.push_back(edgeFace->edgePointIndex + paddingEP );
                            break;
                        }
                    }
                    break;
                }
            }

            catmullFaces.push_back(Face(faceIndices, 4, normal));
        }

    }

    this->object.objectVbo.oFaces.clear();
    this->object.objectVbo.oFaces = catmullFaces;

    for (auto& edgePtr : this->edges) {
        newEdgesForWireframe.push_back(pair(edgePtr->v1, edgePtr->v2));
        delete edgePtr;
    }
    this->object.objectVbo.edgeDataSize = newEdgesForWireframe.size();
    edges.clear();
}


// Function to find faces for each edge
std::unordered_map<Edge, std::vector<int>, EdgeHash> CatmullClark::findFacesForEachEdge(std::vector<Face>& faces) {
    std::unordered_map<Edge, std::vector<int>, EdgeHash> edgeToFaceMap;
    // cout << "Faces size: " << faces.size() << endl;
    for (int i = 0; i < faces.size(); ++i) {
        Face& face = faces[i];
        for (int j = 0; j < face.getSize(); ++j) {
            int v1 = face.vIndex[j];
            int v2 = face.vIndex[(j + 1) % face.getSize()]; // Next vertex in the face, wrap around if it's the last vertex

            Edge edge(v1, v2);
            edgeToFaceMap[edge].push_back(i); // Add the face index to the map for this edge
        }
    }

    return edgeToFaceMap;
}



