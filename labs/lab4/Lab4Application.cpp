#include "Lab4Application.h"
#include "shader4.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Lab4Application* Lab4Application::current_application = nullptr;

Lab4Application::Lab4Application(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {
    currentXSelected = 0;
    currentYSelected = 0;
}

Lab4Application::~Lab4Application() {

}
void Lab4Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_UP:
            getLab4Application()->move(UP);
            break;
        case GLFW_KEY_DOWN:
            getLab4Application()->move(DOWN);
            break;
        case GLFW_KEY_LEFT:
            getLab4Application()->move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            getLab4Application()->move(RIGHT);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            break;
        }
    }
}
void Lab4Application::move(Direction direction) {
    hasMoved = true;
    switch (direction) {
    case UP:
        currentYSelected = (currentYSelected + 1) % numberOfSquare;
        break;
    case DOWN:
        currentYSelected = (currentYSelected - 1) % numberOfSquare;
        break;
    case LEFT:
        currentXSelected = (currentXSelected + 1) % numberOfSquare;
        break;
    case RIGHT:
        currentXSelected = (currentXSelected - 1) % numberOfSquare;
        break;
    default:
        hasMoved = false;
        break;
    }
    //std::cout << "Current X: " << currentXSelected << std::endl;
    //std::cout << "Current Y: " << currentYSelected << std::endl;
}

std::vector<float> Lab4Application::createSelectionSquare() const {

    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.0, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.0, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0, 0, 1, 0, 1
    };

    //print the selection square
    /*for (int i = 0; i < selectionSquare.size(); i+=7) {
        std::cout << selectionSquare[i] << ", " << selectionSquare[i+1] << ", " << selectionSquare[i+2] << std::endl;
    }*/

    return selectionSquare;
}

unsigned Lab4Application::Run() {
    current_application = this;
    //auto triangle = GeometricTools::UnitGrid2D(numberOfSquare);
    auto triangle = GeometricTools::UnitGrid2DWithColor(numberOfSquare); //code with the color not in the shader

    auto selectionSquare = createSelectionSquare();

    triangle.insert(triangle.end(), selectionSquare.begin(), selectionSquare.end());

    //std::cout << triangle.size() << " " << triangle.size() / 7 << std::endl;
    auto vertexArray = std::make_shared<VertexArray>();
    auto indices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto indicesSelectionSquare = GeometricTools::TopologySquare2D;

    for (unsigned i : indicesSelectionSquare) {
        indices.push_back((numberOfSquare + 1) * (numberOfSquare + 1) * 2 + i);
    }

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), sizeof(unsigned int) * indices.size());
    auto gridBufferLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false}
        ,{ShaderDataType::Float4, "color", false} // When we use the color in the vertexBuffer
        }));

    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());


    //auto vertexBufferColor = std::make_shared<VertexBuffer>(color.data(), sizeof(float) * color.size());
    //vertexBufferColor->SetLayout(*gridBufferLayout2);


    vertexBuffer->SetLayout(*gridBufferLayout);
    //vertexArray->AddVertexBuffer(vertexBufferColor);
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    vertexArray->Bind();

    //
    // Shader module
    //
    auto* shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
    shader->Bind();

    //
    // camera
    //
    // perspective on how to observe the world: field of view: 45 degrees, aspect ration of 1, near and far plane of 1 and -10
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, -10.0f);

    // view transformation Matrix: position and orientation of the matrix
    // --> position of the camera ("eye"/position of the camera, Position where the camera is looking at, Normalized up vector)
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // model transformation: scale, rotate, translate (model = scale*rotate*translate)
    // scale: scale matrix, scaling of each axis
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    // rotate: rotation matrix, rotation angle in radians, rotation axis
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    //translate: 
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    auto chessboardModelMatrix = translate * rotate * scale;

    shader->UploadUniformMatrix4fv("u_Model", chessboardModelMatrix);
    shader->UploadUniformMatrix4fv("u_View", viewMatrix);
    shader->UploadUniformMatrix4fv("u_Projection", projectionMatrix);

    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0);
    shader->UploadUniformFloat4("u_Color", color);

    glfwSetKeyCallback(window, Lab4Application::key_callback);


    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        glClearColor(0.663f, 0.663f, 0.663f, 1.0f); // Set clear color to a shade of gray
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the vertex buffer with the new data for the selection square using the currentXSelected and currentYSelected
        if (hasMoved) {
            vertexBuffer->BufferSubData(sizeof(float) * 7 * (numberOfSquare + 1) * (numberOfSquare + 1) * 2,
                sizeof(float) * 7 * 4, createSelectionSquare().data());
            hasMoved = false;
        }
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

unsigned Lab4Application::stop() {
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

Lab4Application* Lab4Application::getLab4Application() {
    return Lab4Application::current_application;
}
