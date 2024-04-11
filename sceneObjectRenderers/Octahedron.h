#include "Object.h"

class Octahedron : public Object
{
public:
    Octahedron(glm::vec3 translation, const string& fileName, glm::vec3 color, const string& vertexShader, const string& fragmentShader, glm::vec3 rotation)
        : Object(translation, fileName, color, vertexShader, fragmentShader, rotation) {}

    void setMatrices(double deltaTime) {
        
        float angleRad = (float) (angle / 180.0) * M_PI;
        float radius = 7.0f;
        float tma = turnAngle * 0.1f * M_PI;
        this->translationMatrix = glm::translate(glm::mat4(1.0), this->translationVec + glm::vec3(0.0f, radius * cos(tma), radius * sin(tma)));
        this->rotationMatrix = glm::rotate<float>(glm::mat4(1.0), angleRad, this->rotationAxis );
        angle += 100 * deltaTime * start_pause_flag;
        turnAngle += 4 * deltaTime * start_pause_flag;
        this->modelingMatrix = this->translationMatrix * this->rotationMatrix  * this->scalingMatrix; // starting from right side, rotate around Y to turn back, then rotate around Z some more at each frame, then translate.

    }

};