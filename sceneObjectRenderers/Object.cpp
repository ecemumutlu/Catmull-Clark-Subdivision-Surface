#include "Object.h"

using namespace std;

// Constructor
Object::Object(glm::vec3& translation, const string& fileName, glm::vec3& color, const string& vertexShader, const string& fragmentShader, glm::vec3& rotation)
		: objectVbo(), 
        program(), // program construction
        translationMatrix(glm::mat4(1.0)),
		scalingMatrix(glm::mat4(1.0)),
		rotationMatrix(glm::mat4(1.0)),
		modelingMatrix(glm::mat4(1.0)),
        projectionMatrix(glm::mat4(1.0)),
		viewingMatrix(glm::mat4(1.0)),
		eyePos(glm::vec3(0, 0, 0)),
		modelingMatrixLoc(-1), 
        viewingMatrixLoc(-1), 
        projectionMatrixLoc(-1), 
        eyePosLoc(-1),
		objectColor(color), 
		objectColorPos(-1),
		level(0),
		translationVec(translation),
		rotationAxis(rotation),
		angle(0.0f),
		mode(0),
		turnAngle(0.0f),
		start_pause_flag(1)
	{ 
        this->program.initShaders(vertexShader, fragmentShader);
        this->objectVbo.parseObjectFromFile(fileName, color);


		this->modelingMatrixLoc = glGetUniformLocation(this->program.gProgram, "modelingMatrix");
        this->viewingMatrixLoc = glGetUniformLocation(this->program.gProgram, "viewingMatrix");
        this->projectionMatrixLoc = glGetUniformLocation(this->program.gProgram, "projectionMatrix");
        this->eyePosLoc = glGetUniformLocation(this->program.gProgram, "eyePos");
        this->objectColorPos = glGetUniformLocation(this->program.gProgram, "uColor");

        this->scalingMatrix = glm::scale(glm::mat4(1.0), glm::vec3(3.0f, 3.0f, 3.0f)); 
    }

Object::~Object() {
	this->objectVbo.~ObjectVbo();
}


void Object::parseObjectFromFile(const string& fileName, glm::vec3 color){
    this->objectVbo.parseObjectFromFile(fileName, color);
}


void Object::initVBO() {
    this->objectVbo.initVBO();
}


void Object::triangulate() {
	this->objectVbo.triangulateQuadFace();
}

void Object::calculateNormals() {
	this->objectVbo.calculateNormals();
}

void Object::drawObject() {
	int vaoInd = this->mode == 0 ? 0 : 1;
	int datasize = this->mode == 0 ? (this->objectVbo.getIndexDataSize() * 3) : (this->objectVbo.edgeDataSize * 2);
	GLenum drawMode = this->mode == 0 ? GL_TRIANGLES : GL_LINES;

	this->program.useProgram();

	glUniformMatrix4fv(this->projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
	glUniformMatrix4fv(this->viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->viewingMatrix));
	glUniformMatrix4fv(this->modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->modelingMatrix));
	glUniform3fv(this->eyePosLoc, 1, glm::value_ptr(this->eyePos));
	glUniform3fv(this->objectColorPos, 1, glm::value_ptr(this->objectColor));

	this->objectVbo.vao[vaoInd].bindVao();
	this->objectVbo.ebo[vaoInd].bindEbo();
	this->objectVbo.vbo[vaoInd].bindVbo();
	
	glDrawElements(drawMode, datasize, GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);

	this->objectVbo.vao[vaoInd].unbindVao();
	this->objectVbo.ebo[vaoInd].unbindEbo();
	this->objectVbo.vbo[vaoInd].unbindVbo();
	

}

void Object::setDepthBuffer() {
	this->program.useProgram();
	glEnable(GL_POLYGON_OFFSET_LINE); // Enable polygon offset for lines
	glPolygonOffset(-5, -30);

	glUniformMatrix4fv(this->projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
	glUniformMatrix4fv(this->viewingMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->viewingMatrix));
	glUniformMatrix4fv(this->modelingMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->modelingMatrix));
	glUniform3fv(this->eyePosLoc, 1, glm::value_ptr(this->eyePos));
	glUniform3fv(this->objectColorPos, 1, glm::value_ptr(this->objectColor));

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Disable writing to the color buffer

	this->objectVbo.vao[0].bindVao();
	this->objectVbo.ebo[0].bindEbo();
	this->objectVbo.vbo[0].bindVbo();
	glDrawElements(GL_TRIANGLES, this->objectVbo.getIndexDataSize() * 3, GL_UNSIGNED_INT, 0);
	assert(glGetError() == GL_NO_ERROR);

	this->objectVbo.vao[0].unbindVao();
	this->objectVbo.ebo[0].unbindEbo();
	this->objectVbo.vbo[0].unbindVbo();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Enable writing to the color buffer

}

void Object::setMatrices(double deltaTime) {
	
	// this->translationMatrix = glm::translate(glm::mat4(1.0), this->translationVec);
    // this->scalingMatrix = glm::scale(glm::mat4(1.0), this->scalingVec);
	float angleRad = (float) (angle / 180.0) * M_PI * start_pause_flag;
    this->translationMatrix = glm::translate(glm::mat4(1.0), this->translationVec);
	this->rotationMatrix = glm::rotate<float>(glm::mat4(1.0), angleRad, this->rotationAxis );
	angle += 3 * deltaTime * start_pause_flag ;
	this->modelingMatrix = this->translationMatrix * this->rotationMatrix  * this->scalingMatrix; // starting from right side, rotate around Y to turn back, then rotate around Z some more at each frame, then translate.

}


void Object::returnBaseVertices() {
	this->objectVbo.returnBaseVertices();
}

void Object::changeMode() {
	mode = (mode + 1) % 3;
}

void Object::resetMatrixVariables() {
	this->angle = 0.0f;
	this->turnAngle = 0.0f;
	this->mode = 0;
}

void Object::start_pause() {
	this->start_pause_flag = (this->start_pause_flag + 1) % 2;
}

void Object::initBuffers() {
	this->objectVbo.initVBO();
	this->objectVbo.initWireframeVBO();
}

void Object::replaceVertices(vector<glm::vec3>& calculatedVertices) {
	this->objectVbo.replaceVertices(calculatedVertices);
}