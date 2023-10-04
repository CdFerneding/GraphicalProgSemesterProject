#include "Lab2Application.h"
#include "shader.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"

Lab2Application::Lab2Application(const std::string &name, const std::string &version, 
    unsigned int width, unsigned int height): GLFWApplication(name, version, width, height) {

}

Lab2Application::~Lab2Application() {

}


unsigned Lab2Application::Run() const {
    auto triangle = GeometricTools::UnitSquare2D;

    //
    // vertex buffer module
    //
    //VertexBuffer vertexbuffer(&triangle, sizeof(float) * triangle.size());
    //vertexbuffer.Bind();
    // Create a vertex array object (VAO)
    GLuint vertexArrayId;
    auto vertexArray = std::make_shared<VertexArray>();
    GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
    };
    //Create the indexBuffer with shared_ptr
    auto indexBuffer = std::make_shared<IndexBuffer>(indices, 6);
    auto gridBufferLayout = BufferLayout({{ShaderDataType::Float2, "position"}});
    auto vertexBuffer = std::make_shared<VertexBuffer>(&triangle, sizeof(float) * triangle.size());

    vertexBuffer->SetLayout(gridBufferLayout);
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    vertexArray->Bind();

    //glGenVertexArrays(1, &vertexArrayId);
    //glBindVertexArray(vertexArrayId);


    //
    // index module
    //
    //IndexBuffer indexBuffer(indices, 6);
    //indexBuffer.Bind();

    // Define the vertex attribute layout of the bound buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);


    //
    // Shader module
    //
    Shader shader(vertexShaderSrc, fragmentShaderSrc);


    while (!glfwWindowShouldClose(window))
    {
        
        //preparation of Window and Shader
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Bind();
        glDrawElements(
                GL_TRIANGLES,       // mode
                6,                  // count
                GL_UNSIGNED_INT,    // type
                (void*)0            // element array buffer offset
        );

        //buffer and drawing
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup of Shader and Buffers
    shader.Unbind();
    shader.~Shader();
    vertexArray->Unbind();
    vertexArray->~VertexArray();

    return 0;
}

