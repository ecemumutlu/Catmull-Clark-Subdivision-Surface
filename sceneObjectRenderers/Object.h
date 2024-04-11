#pragma once
#include "GL/glew.h"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // GL Math library header
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "Program.h"
#include "Structs.h"
#include "ObjectVbo.h"
#include "Face.h"

#define M_PI       3.14159265358979323846   // pi

class Object
{
public:
    glm::mat4 translationMatrix;
	glm::mat4 scalingMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 modelingMatrix;
    glm::mat4 projectionMatrix;
	glm::mat4 viewingMatrix;
	glm::vec3 eyePos;
    glm::vec3 objectColor;
    glm::vec3 translationVec;
    glm::vec3 rotationAxis;
    glm::vec3 transAxis1;
    glm::vec3 transAxis2;
    float angle;
    float turnAngle;
    int level;
    int mode;
    int start_pause_flag;

    Program program;
	GLint modelingMatrixLoc; GLint viewingMatrixLoc; GLint projectionMatrixLoc; GLint eyePosLoc; GLint objectColorPos;

    ObjectVbo objectVbo;

    // Constructor
	Object(glm::vec3& translation, const string& fileName, glm::vec3& color, const string& vertexShader, const string& fragmentShader, glm::vec3& rotation);

    ~Object();

    inline void setProjectionMatrix(glm::mat4 proj) { this->projectionMatrix = proj; }
	inline void setViewingMatrix(glm::mat4 view) { this->viewingMatrix = view; }

    virtual void setMatrices(double deltaTime);

    void drawObject();
    void initVBO();
    void triangulate();
    void parseObjectFromFile(const string& fileName, glm::vec3 color);
    void calculateNormals();
    void returnBaseVertices();
    void changeMode();
    void resetMatrixVariables();
    void start_pause();
    void initBuffers();
    void replaceVertices(vector<glm::vec3>& calculatedVertices);
    void setDepthBuffer();
};
