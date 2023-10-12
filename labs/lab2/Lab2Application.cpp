#include "Lab2Application.h"
#include "shader_lab2.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"

Lab2Application * Lab2Application::current_application = nullptr;

Lab2Application::Lab2Application(const std::string &name, const std::string &version,
                                 unsigned int width, unsigned int height): GLFWApplication(name, version, width, height) {
    currentXSelected = 0;
    currentYSelected = 0;
}

Lab2Application::~Lab2Application() {

}
void Lab2Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_UP:
                getLab2Application()->move(UP);
                break;
            case GLFW_KEY_DOWN:
                getLab2Application()->move(DOWN);
                break;
            case GLFW_KEY_LEFT:
                getLab2Application()->move(LEFT);
                break;
            case GLFW_KEY_RIGHT:
                getLab2Application()->move(RIGHT);
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                break;
        }
    }
}
void Lab2Application::move(Direction direction) {
    switch (direction) {
        case UP:
            currentYSelected = (currentYSelected + 1) % numberOfSquare;
            break;
        case DOWN:
            currentYSelected = (currentYSelected + 7) % numberOfSquare;
            break;
        case LEFT:
            currentXSelected = (currentXSelected + 7) % numberOfSquare;
            break;
        case RIGHT:
            currentXSelected = (currentXSelected + 1) % numberOfSquare;
            break;
    }
    std::cout << "Current X: " << currentXSelected << std::endl;
    std::cout << "Current Y: " << currentYSelected << std::endl;
}

unsigned Lab2Application::Run() {
    current_application = this;
    auto triangle = GeometricTools::UnitGrid2D(numberOfSquare);

    auto vertexArray = std::make_shared<VertexArray>();
    auto indices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), sizeof(unsigned int) * indices.size());
    auto gridBufferLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "OpenGLLayout", false},
        {ShaderDataType::Float3, "position"}
    }));
    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());

    vertexBuffer->SetLayout(*gridBufferLayout);
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
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    //glEnableVertexAttribArray(0);
    //
    // Shader module
    //
    auto * shader = new Shader(vertexShaderSrc, fragmentShaderSrc);

    glfwSetKeyCallback(window, Lab2Application::key_callback);


    while (!glfwWindowShouldClose(window))
    {
        //preparation of Window and Shader
        glClear(GL_COLOR_BUFFER_BIT);
        shader->Bind();
        glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_INT,   // type
                nullptr           // element array buffer offset
        );

        //buffer and drawing
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup of Shader and Buffers
    shader->Unbind();
    vertexArray->Unbind();

    return stop();
}

unsigned Lab2Application::stop() {
    unsigned code = GLFWApplication::stop();
    if (GLFWApplication::stop() != EXIT_SUCCESS) {
        //print error message
        std::cerr << "Error stopping GLFW application" << std::endl;
        std::cerr << "Error code: " << code << std::endl;
        //print opengl message
        std::cerr << "OpenGL Error: " << glGetError() << std::endl;
    }
    return code;
}

Lab2Application * Lab2Application::getLab2Application() {
    return Lab2Application::current_application;
}
