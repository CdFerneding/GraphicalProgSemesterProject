#include <iostream>
#include "VertexBuffer.h"

int VertexBuffer::count = 0;

VertexBuffer::VertexBuffer(const void *vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);
    Bind();
    // transfer data to GPU
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    count++;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VertexBufferID);
    count--;
    // cout << "new number of triangles is: " << triangle_counter << endl;
}

void VertexBuffer::Bind() const
{
    //Iterate over Layout.begin()
    unsigned countBufferLayout = 0;
    unsigned offset = 0;
    for (auto i = Layout.begin(); i != Layout.end(); ++i) {
        // get the attribute
        auto attribute = *i;
        glVertexAttribPointer(countBufferLayout, ShaderDataTypeComponentCount(attribute.Type),
                              ShaderDataTypeToOpenGLBaseType(attribute.Type), attribute.Normalized,
                              (int) attribute.Size, (const void *) offset);

        std::cout << "glVertextAttribPointer(" << countBufferLayout << ", " << ShaderDataTypeComponentCount(attribute.Type)
                    << ", " << ShaderDataTypeToOpenGLBaseType(attribute.Type) << ", " << attribute.Normalized << ", "
                    << attribute.Size << ", " << offset << ")" << std::endl;

        glEnableVertexAttribArray(countBufferLayout);
        offset += attribute.Size;
        //std::cout << "glEnableVertexAttribArray(" << countBufferLayout << ")" << std::endl;
        countBufferLayout++;
    }

    // make the vertex buffer active
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data) const
{
    // Update a specific segment of the buffer with new data
    // replacement starts at "offset"-bytes into the array
    // replacement goes "size"-bytes far
    // this part will be replaced by data
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
