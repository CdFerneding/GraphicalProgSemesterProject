#include "VertexBuffer.h"

int VertexBuffer::triangle_counter = 0;

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    // Generate a Vertex Buffer Object (VBO) and bind it
    glGenBuffers(1, &VertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

    // Allocate memory for the buffer and copy the data
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    triangle_counter++;
    // cout << "new number of triangles is: " << triangle_counter << endl;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VertexBufferID);
    triangle_counter--;
    // cout << "new number of triangles is: " << triangle_counter << endl;
}

void VertexBuffer::Bind() const
{
    // Bind the Vertex Buffer Object to GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    // Unbind any currently bound GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const
{
    Bind();
    // Update a specific segment of the buffer with new data
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    
    Unbind();
}
