
#include <GL/glew.h>
#include <cassert>
class Vao {
    public:
        GLuint vaoId;

        Vao();
        void bindVao();
        void unbindVao();
        void deleteVao();
};