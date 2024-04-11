#include "Ebo.h"

Ebo::Ebo() {
    glGenBuffers(1, &eboId);
    assert(eboId > 0);
}

void Ebo::bindEbo() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
}

void Ebo::unbindEbo() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Ebo::deleteEbo() {
    glDeleteBuffers(1, &eboId);
    eboId = 0;
}