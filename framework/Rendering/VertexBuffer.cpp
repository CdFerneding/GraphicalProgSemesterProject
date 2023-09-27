#include "VertexBuffer.h"

int VertexBuffer::triangle_counter = 0;

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    triangle_counter++;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(triangle_counter, &VertexBufferID);
    // cout << "new number of triangles is: " << triangle_counter << endl;
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const
{
    // Update a specific segment of the buffer with new data
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
