#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GLuint *indices, GLsizei count) {
    this->Count = count;
    glGenBuffers(count, &(this->IndexBufferID));
    glBindBuffer(GL_ARRAY_BUFFER, this->IndexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * count, indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(Count, &IndexBufferID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, IndexBufferID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}