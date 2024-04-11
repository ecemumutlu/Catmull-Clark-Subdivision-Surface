#include <GL/glew.h>
#include <cassert>

class Vbo {
    public:
        GLuint vboId;

        Vbo();
        void bindVbo();
        void unbindVbo();
        void deleteVbo();
};