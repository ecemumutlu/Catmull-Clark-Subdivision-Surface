#include "Object.h"

class Cube : public Object
{
public:
    Cube(glm::vec3 translation, const string& fileName, glm::vec3 color, const string& vertexShader, const string& fragmentShader, glm::vec3 rotation)
        : Object(translation, fileName, color, vertexShader, fragmentShader, rotation) {}


    void setMatrices(double deltaTime) override{
        // this->translationMatrix = glm::translate(glm::mat4(1.0), this->translationVec);
        // this->scalingMatrix = glm::scale(glm::mat4(1.0), this->scalingVec);
        float angleRad = (float) (angle / 180.0) * M_PI;
        this->translationMatrix = glm::translate(glm::mat4(1.0), this->translationVec );
        this->rotationMatrix = glm::rotate<float>(glm::mat4(1.0), angleRad, this->rotationAxis );
        angle += 100 * deltaTime * start_pause_flag;
        this->modelingMatrix = this->translationMatrix * this->rotationMatrix  * this->scalingMatrix; // starting from right side, rotate around Y to turn back, then rotate around Z some more at each frame, then translate.

    }
};