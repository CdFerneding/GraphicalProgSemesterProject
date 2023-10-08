#include "VertextArray.h"

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_vertexArrayID);
    for (auto &vertexBuffer : VertexBuffers) {
        vertexBuffer->Bind();
    }
    IdxBuffer->Bind();
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray() {
    IdxBuffer->~IndexBuffer();
    for (auto &vertexBuffer : VertexBuffers) {
        vertexBuffer->~VertexBuffer();
    }
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
    IdxBuffer->Unbind();

    for (auto &vertexBuffer : VertexBuffers) {
        vertexBuffer->Unbind();
    }
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    IdxBuffer = indexBuffer;
}
