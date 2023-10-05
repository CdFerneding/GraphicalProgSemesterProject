#include "Lab2Application.h"
#include "shader.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    // camera
    //
    // perspective on how to observe the world: field of view: 45 degrees, aspect ration of 1, near and far plane of 1 and -10
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 1.0f, -10.0f);

    // view transformation Matrix: position and orientation of the matrix
    // --> position of the camera ("eye", Position where the camera is looking at, Normalized up vektor
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0,0,0), glm::vec3(0,1,0)); 
    
    // model transformation: scale, rotate, translate (model = scale*rotate*translate)
    // scale: scale matrix, scaling of each axis
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    // rotate: rotation matrix, rotation angle in radians, rotation axis
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 0.0f, 1.0f ));

    //translate: 
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));

    auto chessboardModelMatrix = translate * rotate * scale; 

    // extend shader to support 4x4 matrices as uniforms
    // Get the location of our uniforms in the shader
    auto u_Projection = glGetUniformLocation(vertexShaderSrc, "u_Projection");
    auto u_View = glGetUniformLocation(vertexShaderSrc, "u_View");
    auto u_Model = glGetUniformLocation(vertexShaderSrc, "u_Model");
    
    glUniformMatrix4fv(u_Projection, 1, GL_FALSE, projectionMatrix);
    glUniformMatrix4fv(u_View, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, chessboardModelMatrix);

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

