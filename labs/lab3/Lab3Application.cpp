#include "Lab3Application.h"
#include "shader.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "../../framework/ErrorHandling/ErrorHandling.h"

Lab3Application::Lab3Application(const std::string &name, const std::string &version,
                                 unsigned int width, unsigned int height): GLFWApplication(name, version, width, height) {

}

Lab3Application::~Lab3Application() {

}


unsigned Lab3Application::Run() {
    unsigned numberOfSquare = 8;
    auto triangle = GeometricTools::UnitGrid2D(numberOfSquare);

    auto vertexArray = std::make_shared<VertexArray>();
    auto indices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), sizeof(unsigned int) * indices.size());
    auto gridBufferLayout = BufferLayout({ {ShaderDataType::Float3, "position"} });
    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);


    //
    // Shader module
    //
    Shader shader(vertexShaderSrc, fragmentShaderSrc);


    //
    // camera
    //
    // perspective on how to observe the world: field of view: 45 degrees, aspect ration of 1, near and far plane of 1 and -10
    glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 1.0f, -10.0f);

    // view transformation Matrix: position and orientation of the matrix
    // --> position of the camera ("eye"/position of the camera, Position where the camera is looking at, Normalized up vektor)
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // model transformation: scale, rotate, translate (model = scale*rotate*translate)
    // scale: scale matrix, scaling of each axis
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    // rotate: rotation matrix, rotation angle in radians, rotation axis
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    //translate: 
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));

    auto chessboardModelMatrix = translate * rotate * scale;

    // Get the location of our uniforms in the shader
    auto u_Model = glGetUniformLocation(shader.ShaderProgram, "u_Model");
    auto u_View = glGetUniformLocation(shader.ShaderProgram, "u_View"); 
    auto u_Projection = glGetUniformLocation(shader.ShaderProgram, "u_Projection");

    // Set uniform matrix values
    glUniformMatrix4fv(u_Model, 1, GL_FALSE, glm::value_ptr(chessboardModelMatrix));
    glUniformMatrix4fv(u_View, 1, GL_FALSE, glm::value_ptr(viewMatrix)); 
    glUniformMatrix4fv(u_Projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Bind();
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_INT,   // type
            (void*)0           // element array buffer offset
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
    //vertexArray->~VertexArray();
    unsigned int stop_error_code = stop();
    if (stop_error_code != EXIT_SUCCESS) {
        //print error message
        std::cerr << "Error stopping GLFW application" << std::endl;
        std::cerr << "Error code: " << stop_error_code << std::endl;
        //print opengl message
        std::cerr << "OpenGL Error: " << glGetError() << std::endl;
    }

    return stop_error_code;
}

