#include "Lab2Application.h"
#include "shader_lab2.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include "RenderCommands.h"

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

std::vector<float> Lab2Application::createSelectionSquare() const {

    std::vector<float> selectionSquare = {
            1 - (2.0f/(float) numberOfSquare)*(float (currentXSelected)), -1 + (2.0f/(float) numberOfSquare)*(float (currentYSelected)), 0.1, 0, 1, 0, 1,
            1 - (2.0f/(float) numberOfSquare)*(float (currentXSelected)), -1 + (2.0f/(float) numberOfSquare)*(float (currentYSelected+1)), 0.1, 0, 1, 0, 1,
            1 - (2.0f/(float) numberOfSquare)*(float (currentXSelected+1)), -1 + (2.0f/(float) numberOfSquare)*(float (currentYSelected+1)),  0.1, 0, 1, 0, 1,
            1 - (2.0f/(float) numberOfSquare)*(float (currentXSelected+1)), -1 + (2.0f/(float) numberOfSquare)*(float (currentYSelected)), 0.1, 0, 1, 0, 1
    };

    //print the selection square
    /*for (int i = 0; i < selectionSquare.size(); i+=7) {
        std::cout << selectionSquare[i] << ", " << selectionSquare[i+1] << ", " << selectionSquare[i+2] << std::endl;
    }*/

    return selectionSquare;
}

unsigned Lab2Application::Run() {
    current_application = this;
    //auto triangle = GeometricTools::UnitGrid2D(numberOfSquare);
    auto triangle = GeometricTools::UnitGrid2DWithColor(numberOfSquare); //code with the color not in the shader

    auto selectionSquare = createSelectionSquare();

    triangle.insert(triangle.end(), selectionSquare.begin(), selectionSquare.end());

    //std::cout << triangle.size() << " " << triangle.size() / 7 << std::endl;
    auto vertexArray = std::make_shared<VertexArray>();
    auto indices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto indicesSelectionSquare = GeometricTools::TopologySquare2D;

    for(unsigned i : indicesSelectionSquare) {
        indices.push_back((numberOfSquare+1) * (numberOfSquare+1) * 2 + i);
    }

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    auto gridBufferLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false}
        ,{ShaderDataType::Float4, "color", false} // When we use the color in the vertexBuffer
    }));

    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());

    auto cube = GeometricTools::UnitCubeWithColor(1,0,0,1);

    auto vertexArrayCube = std::make_shared<VertexArray>();

    auto indicesCube = GeometricTools::CubeTopology;

    auto indexBufferCube = std::make_shared<IndexBuffer>(indicesCube.data(), indicesCube.size());

    auto vertexBufferCube = std::make_shared<VertexBuffer>(cube.data(), sizeof(unsigned int) * cube.size());

    vertexBufferCube->SetLayout(*gridBufferLayout);
    vertexArrayCube->AddVertexBuffer(vertexBufferCube);
    vertexArrayCube->SetIndexBuffer(indexBufferCube);

    vertexArrayCube->Bind();

    vertexBuffer->SetLayout(*gridBufferLayout);
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    vertexArray->Bind();

    auto * shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
    shader->Bind();

    glfwSetKeyCallback(window, Lab2Application::key_callback);


    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // Set clear color to a shade of gray
        RenderCommands::Clear();

        // Update the vertex buffer with the new data for the selection square using the currentXSelected and currentYSelected
        if(hasMoved) {
            vertexBuffer->BufferSubData(sizeof(float) * 7 * (numberOfSquare + 1) * (numberOfSquare + 1) * 2,
                                        sizeof(float) * 7 * 4, createSelectionSquare().data());
            hasMoved = false;
        }

        RenderCommands::DrawIndex(
                GL_TRIANGLES,
                vertexArray
        );

        RenderCommands::DrawIndex(
                GL_TRIANGLES,
                vertexArrayCube
        );

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup of Shader and Buffers
    shader->Unbind();
    vertexArray->Unbind();
    vertexArrayCube->Unbind();

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
