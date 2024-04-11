#include "Vbo.h"

Vbo::Vbo() {
    glGenBuffers(1, &vboId);
    assert(vboId > 0);
}

void Vbo::bindVbo() {
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
}

void Vbo::unbindVbo() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vbo::deleteVbo() {
    glDeleteBuffers(1, &vboId);
    vboId = 0;
}