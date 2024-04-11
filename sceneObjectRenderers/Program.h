#include <GL/glew.h>
#include <cstring>
#include <string>
using namespace std;

class Program {

private:
    GLuint createVS(const string& shaderName);
    GLuint createFS(const string& shaderName);  
    bool ReadDataFromFile(const string& fileName, string& data);

public:
    GLuint gProgram;
    GLuint vs;
    GLuint fs_normal;
    GLuint fs_ground; 

    Program() : gProgram(0), vs(0), fs_normal(0), fs_ground(0) {};

    ~Program() { };

    void initShaders(const string& vertexShader, const string& fragmentShader);
    void useProgram();
};



