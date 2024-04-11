#include "ObjectVbo.h"
#include <set>
#include <unordered_map>
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

ObjectVbo::~ObjectVbo() {
	oVertices.clear();
	oNormals.clear();
	oFaces.clear();
	oQuadFaces.clear();
	baseVertices.clear();
	baseFaces.clear();

	for (auto& v: vao) {
		v.deleteVao();
	}
	vao.clear();

	for (auto& e: ebo) {
		e.deleteEbo();
	}
	ebo.clear();

	for (auto& vbo: vbo) {
		vbo.deleteVbo();
	}
	vbo.clear();
}

int ObjectVbo::getVertexDataSize() { 
    return oVertices.size(); 
}

int ObjectVbo::getNormalDataSize() {
    return oNormals.size();
}


int ObjectVbo::getIndexDataSize() {
    return oFaces.size();
}


void ObjectVbo::replaceVertices(vector<glm::vec3>& calculatedVertices) {
    this->oVertices.clear();
    for (auto& v: calculatedVertices) {
        this->oVertices.push_back(Vertex(v));
    }
}


void ObjectVbo::initVBO(){
    
    vao[0].bindVao();

    glEnableVertexAttribArray(0); //location 0
    glEnableVertexAttribArray(1); //location 1

    assert(glGetError() == GL_NONE);

    vbo[0].bindVbo();
    ebo[0].bindEbo();

	std::vector<Vertex> newVertices;
	std::vector<Normal> newNormals;
	std::vector<Face> newFaces;

	for (auto& f: this->oFaces) {
		newNormals.insert(newNormals.end(), 3, f.getFaceNormal());
		int position = newVertices.size();

		for (auto& index: f.vIndex) {
			newVertices.push_back(this->oVertices[index]);
		}
		int v[] = {position, position + 1, position + 2};
		newFaces.push_back(Face(v, 3));
	}

    int oVerticesSize = newVertices.size();
    int oNormalsSize = newNormals.size();
    int oFacesSize = newFaces.size();

    int gVertexDataSizeInBytes = oVerticesSize * 3 * sizeof(GLfloat);
    int gNormalDataSizeInBytes = oNormalsSize * 3 * sizeof(GLfloat);
    int gIndexDataSizeInBytes = oFacesSize * 3 * sizeof(GLuint);

    GLfloat* vertexData = new GLfloat[oVerticesSize * 3];
    GLfloat* normalData = new GLfloat[oNormalsSize * 3];
    GLuint* indexData = new GLuint[oFacesSize * 3];

    for (int i = 0; i < oVerticesSize; ++i)
    {
        vertexData[3 * i] =  newVertices[i].x;
        vertexData[3 * i + 1] =  newVertices[i].y;
        vertexData[3 * i + 2] =  newVertices[i].z;
    }

    for (int i = 0; i < oNormalsSize; ++i)
    {
        normalData[3 * i] = newNormals[i].x;
        normalData[3 * i + 1] = newNormals[i].y;
        normalData[3 * i + 2] = newNormals[i].z;
    }

    for (int i = 0; i < oFacesSize; ++i)
    {
        indexData[3 * i] = newFaces[i].vIndex[0];
        indexData[3 * i + 1] = newFaces[i].vIndex[1];
        indexData[3 * i + 2] = newFaces[i].vIndex[2];

    }


    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gIndexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // done copying to GPU memory; can free now from CPU memory
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

    vao[0].unbindVao();
    vbo[0].unbindVbo();
    ebo[0].unbindEbo();
}


void ObjectVbo::initWireframeVBO(){
    
    vao[1].bindVao();

    glEnableVertexAttribArray(0); //location 0
    glEnableVertexAttribArray(1); //location 1

    assert(glGetError() == GL_NONE);

    //---------------

    vbo[1].bindVbo();
    ebo[1].bindEbo();

    std::unordered_map<Edge, std::vector<int>, EdgeHash> edgeToFaceMap;
    // cout << "Faces size: " << faces.size() << endl;
    for (int i = 0; i < oQuadFaces.size(); ++i) {
        Face& face = oQuadFaces[i];
        for (int j = 0; j < face.getSize(); ++j) {
            int v1 = face.vIndex[j];
            int v2 = face.vIndex[(j + 1) % face.getSize()]; // Next vertex in the face, wrap around if it's the last vertex

            Edge edge(v1, v2);
            edgeToFaceMap[edge].push_back(i); // Add the face index to the map for this edge
			
        }
    }

    vector<Vertex> newVertices;
    vector<Normal> newNormals;
	vector<pair<int, int>> newEdgesForWireframe;
    for (auto& entry : edgeToFaceMap) {
        Edge tmpEdge = entry.first;
        std::vector<int>& faceIndices = entry.second;

        newNormals.insert(newNormals.end(), 2 , Normal(this->oQuadFaces[faceIndices[0]].getFaceNormal()));
        newNormals.insert(newNormals.end(), 2 , Normal(this->oQuadFaces[faceIndices[1]].getFaceNormal()));

        int position = newVertices.size();
        newVertices.push_back(this->oVertices[tmpEdge.v1]);
        newVertices.push_back(this->oVertices[tmpEdge.v2]);
        newVertices.push_back(this->oVertices[tmpEdge.v1]);
        newVertices.push_back(this->oVertices[tmpEdge.v2]);

		newEdgesForWireframe.push_back(pair(position, position + 1));
        newEdgesForWireframe.push_back(pair(position + 2, position + 3));
	}
	
    int oNormalsSize = newNormals.size();
   	int oVerticesSize = newVertices.size();
	int oEdgesSize = newEdgesForWireframe.size();

	this->edgeDataSize = oEdgesSize;

    int gVertexDataSizeInBytes = oVerticesSize * 3 * sizeof(GLfloat);
    int gNormalDataSizeInBytes = oNormalsSize * 3 * sizeof(GLfloat);
    int gEdgeDataSizeInBytes = oEdgesSize * 2 * sizeof(GLuint);

    GLfloat* normalData = new GLfloat[oNormalsSize * 3];
    GLfloat* vertexData = new GLfloat[oVerticesSize * 3];
    GLuint* edgeIndexData = new GLuint[oEdgesSize * 2];


    for (int i = 0; i < oVerticesSize; ++i)
    {
        vertexData[3 * i] =  newVertices[i].x;
        vertexData[3 * i + 1] =  newVertices[i].y;
        vertexData[3 * i + 2] =  newVertices[i].z;
    }

    for (int i = 0; i < oEdgesSize; ++i)
    {
        edgeIndexData[2 * i] = newEdgesForWireframe[i].first;
        edgeIndexData[2 * i + 1] = newEdgesForWireframe[i].second;
    }

    for (int i = 0; i < oNormalsSize; ++i)
    {
        normalData[3 * i] = newNormals[i].x;
        normalData[3 * i + 1] = newNormals[i].y;
        normalData[3 * i + 2] = newNormals[i].z;
    }
	

    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gEdgeDataSizeInBytes, edgeIndexData, GL_STATIC_DRAW);

    // done copying to GPU memory; can free now from CPU memory
    delete[] vertexData;
    delete[] edgeIndexData;
    delete[] normalData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));

    vao[1].unbindVao();
    vbo[1].unbindVbo();
    ebo[1].unbindEbo();
}


void ObjectVbo::parseObjectFromFile(const string& fileName, glm::vec3 color){

	fstream myfile;

	// Open the input 
	myfile.open(fileName.c_str(), std::ios::in);


	if (myfile.is_open())
	{
		string curLine;

		while (getline(myfile, curLine))
		{
			stringstream str(curLine);
			GLfloat c1, c2, c3;
			GLuint index[9];
			string tmp;

			if (curLine.length() >= 2)
			{
				if (curLine[0] == 'v')
				{
					if (curLine[1] == 't') // texture
					{
						str >> tmp; // consume "vt"
						str >> c1 >> c2;
					}
					else if (curLine[1] == 'n') // normal
					{
						str >> tmp; // consume "vn"
						str >> c1 >> c2 >> c3;
						// this->oNormals.push_back(Normal(c1, c2, c3));
					}
					else // vertex
					{
						str >> tmp; // consume "v"
						str >> c1 >> c2 >> c3;
						this->oVertices.push_back(Vertex(c1 , c2 , c3 ));
					}
				}
				else if (curLine[0] == 'f') // face
				{
					str >> tmp; // consume "f"
					char c;
					int index;
					vector<int> vIndex, tIndex, nIndex;

					// assert(vIndex[0] == nIndex[0] &&
					// 	vIndex[1] == nIndex[1] &&
					// 	vIndex[2] == nIndex[2]  
					// 	// && vIndex[3] == nIndex[3]
					// 	); // a limitation for now

					while ( str >> index) {
						vIndex.push_back(index);
						str >> c >> c; // consume "//"
						str >> index;
						// nIndex.push_back(index);
					}

					int faceSize = vIndex.size();
					int v[faceSize], n[faceSize], t[faceSize];
					for(int i = 0; i < faceSize; i++) {
						v[i] = vIndex[i];
						// n[i] = nIndex[i];
					}


					// make indices start from 0
					for (int c = 0; c < faceSize; c++)
					{
						v[c]--;
						// n[c]--;
					}
					
					this->oFaces.push_back(Face(v, faceSize));
					// if (faceSize == 4) {
					// 	this->oQuadFaces.push_back(QuadFace(v, n));
					// }
					
				}
				else
				{
					//cout << "Ignoring unidentified line in obj file: " << curLine << endl;
				}
			}
			//data += curLine;
			if (!myfile.eof())
			{
				//data += "\n";
			}
		}

		myfile.close();
	}
	else
	{
		cout << "Cannot find file name: " + fileName << endl;
		// return false;
	}

	for (auto& face: this->oFaces) {
		glm::vec3 normal = glm::normalize(glm::cross(
			this->oVertices[face.vIndex[1]].toVec3() - this->oVertices[face.vIndex[0]].toVec3(),
			this->oVertices[face.vIndex[2]].toVec3() - this->oVertices[face.vIndex[0]].toVec3()
		));

		face.setFaceNormal(normal);
	}

    for (auto& v: oVertices) {
        baseVertices.push_back(glm::vec3(v.x, v.y, v.z));
    }

	baseFaces.insert(baseFaces.end(), oFaces.begin(), oFaces.end());

}


void ObjectVbo::triangulateQuadFace(){
	// triangulate quad face
	vector<Face> newFaces;
	int faceCount = this->oFaces.size();

	if (faceCount == 0 || this->oFaces[0].size == 3) {
		this->oQuadFaces.clear();
		this->oQuadFaces.insert(this->oQuadFaces.end(), this->oFaces.begin(), this->oFaces.end());
		return;
	}

	for (int i = 0; i < faceCount; ++i)
	{		
		Face f = this->oFaces[i];
		GLuint vIndex1[3] = {f.vIndex[0], f.vIndex[1], f.vIndex[2]};

		GLuint vIndex2[3] = {f.vIndex[2], f.vIndex[3], f.vIndex[0]};

		// Add Face instances to newFaces vector using GLuint vectors
		Face face1(vIndex1, 3, f.getFaceNormal());
		Face face2(vIndex2, 3, f.getFaceNormal());

		newFaces.push_back(face1);
		newFaces.push_back(face2);
	}
	this->oQuadFaces.clear();
	this->oQuadFaces.insert(this->oQuadFaces.end(), this->oFaces.begin(), this->oFaces.end());
	this->oFaces.clear();
	this->oFaces = newFaces;
}


 void ObjectVbo::calculateNormals() {
	vector<Vertex> newVertices;
	for (auto& f: this->oFaces) {
		this->oNormals.insert(this->oNormals.end(), 3, f.getFaceNormal());
		int position = newVertices.size();

		for (auto& index: f.vIndex) {
			newVertices.push_back(this->oVertices[index]);
		}

		f.setVIndex(position, position + 1, position + 2);
	}
	this->oVertices.clear();
	this->oVertices = newVertices;
	assert(this->oNormals.size() == this->oVertices.size());
 }


vector<Face> ObjectVbo::getFaces() {
	return this->oFaces;
}


vector<Vertex>& ObjectVbo::getVertices() {
	return this->oVertices;
}

int ObjectVbo::getFaceSize() {
	return this->oFaces.size();
}

vector<Face>& ObjectVbo::getQuadFaces() {
	return this->oQuadFaces;
}

int ObjectVbo::getQuadFaceSize() {
	return this->oQuadFaces.size();
}

void ObjectVbo::returnBaseVertices() {
    this->oVertices.clear();
    for (auto& v: this->baseVertices) {
        this->oVertices.push_back(Vertex(v));
    }

	this->oFaces.clear();
	this->oFaces.insert(this->oFaces.end(), this->baseFaces.begin(), this->baseFaces.end());

	this->oQuadFaces.clear();
	this->oNormals.clear();

	this->triangulateQuadFace();

}



