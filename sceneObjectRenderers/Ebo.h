#include <GL/glew.h>
#include <cassert>
class Ebo {
    public:
        GLuint eboId;

        Ebo();
        void bindEbo();
        void unbindEbo();
        void deleteEbo();
};