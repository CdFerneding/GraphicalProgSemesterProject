#include "VertextArray.h"

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_vertexArrayID);
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray() {

}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    IdxBuffer = indexBuffer;
}
