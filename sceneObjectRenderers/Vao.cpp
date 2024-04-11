#include "Vao.h"


Vao::Vao() {
    glGenVertexArrays(1, &vaoId);
    assert(vaoId > 0);
}


void Vao::bindVao() {
    glBindVertexArray(vaoId);
}

void Vao::unbindVao() {
    glBindVertexArray(0);
}
void Vao::deleteVao() {
    glDeleteVertexArrays(1, &vaoId);
    vaoId = 0;
}