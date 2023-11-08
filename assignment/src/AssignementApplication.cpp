#include "AssignementApplication.h"
#include "shader.h"
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <RenderCommands.h>
#include <unordered_map>

AssignementApplication* AssignementApplication::current_application = nullptr;

AssignementApplication::AssignementApplication(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {
    currentXSelected = 0;
    currentYSelected = 0;
    rotationAngleX = 0;
    rotationAngleY = 0;
    currentRotationAngleX = 0;
    currentRotationAngleY = 0;
}

AssignementApplication::~AssignementApplication() {

}
void AssignementApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_UP:
            getAssignementApplication()->move(UP);
            break;
        case GLFW_KEY_DOWN:
            getAssignementApplication()->move(DOWN);
            break;
        case GLFW_KEY_LEFT:
            getAssignementApplication()->move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            getAssignementApplication()->move(RIGHT);
            break;

        // There is two key for up and left because we are using a QWERTY and an AZERTY keyboard
        case GLFW_KEY_Z:
        case GLFW_KEY_W:
		    //getAssignementApplication()->rotateCube(UP);
		    break;
        case GLFW_KEY_A:
            //getAssignementApplication()->rotateCube(LEFT);
            break;
        case GLFW_KEY_Q:
            getAssignementApplication()->exit();
            break;
        case GLFW_KEY_S:
            //getAssignementApplication()->rotateCube(DOWN);
            break;
        case GLFW_KEY_D:
            //getAssignementApplication()->rotateCube(RIGHT);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_ENTER:
            getAssignementApplication()->select();
            break;
        case GLFW_KEY_P:
            getAssignementApplication()->zoom(0.2f);
            break;
        case GLFW_KEY_O:
            getAssignementApplication()->zoom(-0.2f);
            break;
        case GLFW_KEY_L:
            getAssignementApplication()->rotate(15);
            break;
        case GLFW_KEY_H:
            getAssignementApplication()->rotate(-15);
            // counter-clockwise rotation
            break;
        default:
            break;
        }
    }
}
void AssignementApplication::move(Direction direction) {
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

void AssignementApplication::rotate(float degree) {
    hasCameraChanged = true;
}

void AssignementApplication::zoom(float zoomValue) {
    hasCameraChanged = true;
}

void AssignementApplication::select() {
     hasCubeSelected = true;
}

void AssignementApplication::updateSelectedCube() {
    auto current_cube = cubes.find(currentXSelected*7+currentYSelected);
    std::array<int, 2> new_coordinate = {static_cast<int>(currentXSelected), static_cast<int>(currentYSelected)};
    if (currentCubeSelected != -1) {
        if (current_cube != nullptr) {
            new_coordinate = previousPosition;
            std::cout << "Déplacement du cube sur la précédente position" << std::endl;
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(currentSelectedVertexArray->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(colorSelectedCube ? 0 : 1, 0.0f, colorSelectedCube ? 1 : 0, new_coordinate[0], new_coordinate[1]);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            previousPosition = {static_cast<int>(-1), static_cast<int>(-1)};
        }else {
            std::cout << "Déplacement du cube" << std::endl;
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(currentSelectedVertexArray->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(colorSelectedCube ? 0 : 1, 0.0f, colorSelectedCube ? 1 : 0, new_coordinate[0], new_coordinate[1]);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            previousPosition = {static_cast<int>(-1), static_cast<int>(-1)};
            cubes.insert({currentXSelected * numberOfSquare + currentYSelected, currentSelectedVertexArray});
            cubes.erase(currentCubeSelected);
        }
        currentCubeSelected = -1;

    }else {
        if (current_cube != nullptr) {
            auto the_cube = current_cube->second;
            currentCubeSelected = currentXSelected * numberOfSquare + currentYSelected;
            auto vertexBuffer = the_cube->getVertexBuffer(the_cube->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            colorSelectedCube = cubesColor[currentXSelected * numberOfSquare + currentYSelected];
            auto cube = createSelectionCube(0.0f, 1.0f, 0.0f, currentXSelected, currentYSelected);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            currentSelectedVertexArray = the_cube;
            std::cout << "Sélection du cube" << std::endl;
            previousPosition = new_coordinate;
        }
    }
}

std::vector<float> AssignementApplication::createSelectionSquare() const {

    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.0001, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.0001, 0, 1, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, 1
    };

    //print the selection square
    /*for (int i = 0; i < selectionSquare.size(); i+=7) {
        std::cout << selectionSquare[i] << ", " << selectionSquare[i+1] << ", " << selectionSquare[i+2] << std::endl;
    }*/

    return selectionSquare;
}

std::vector<float> AssignementApplication::createSelectionCube(float r, float g, float b, unsigned int x, unsigned int y) const {
    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(x)) - 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y)) + 1.0f / ((float) numberOfSquare*2.0f), 0.1, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x)) - 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y + 1)) - 1.0f / ((float) numberOfSquare*2.0f), 0.1, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x + 1)) + 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y + 1)) - 1.0f / ((float) numberOfSquare*2.0f),  0.1, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x + 1)) + 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y)) + 1.0f / ((float) numberOfSquare*2.0f), 0.1, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x)) - 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y)) + 1.0f / ((float) numberOfSquare*2.0f), 2.0f / (float)numberOfSquare, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x)) - 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y + 1)) - 1.0f / ((float) numberOfSquare*2.0f), 2.0f / (float)numberOfSquare, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x + 1)) + 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y + 1)) - 1.0f / ((float) numberOfSquare*2.0f),  2.0f / (float)numberOfSquare, r, g, b, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(x + 1)) + 1.0f / ((float) numberOfSquare*2.0f), -1 + (2.0f / (float)numberOfSquare) * (float(y)) + 1.0f / ((float) numberOfSquare*2.0f), 2.0f / (float)numberOfSquare, r, g, b, 1
    };
    return selectionSquare;
}


unsigned AssignementApplication::Run() {
    current_application = this;
    hasCameraChanged = false;
    hasMoved = false;

    auto triangle = GeometricTools::UnitGrid2DWithColor(numberOfSquare); //code with the color not in the shader

    auto selectionSquare = createSelectionSquare();

    auto vertexArray = std::make_shared<VertexArray>();
    auto indices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto indicesSelectionSquare = GeometricTools::TopologySquare2D;

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    auto gridBufferLayout = std::make_shared<BufferLayout>(BufferLayout({
            {ShaderDataType::Float3, "position", false},
            {ShaderDataType::Float4, "color",    false} // When we use the color in the vertexBuffer
    }));

    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());

    std::vector<std::vector<float>> arrayColor = {
            {1.0f, 0.0f, 0.0f}, //red
            {0.0f, 0.0f, 1.0f} //blue
    };

    auto vertexBufferSelectionSquare = std::make_shared<VertexBuffer>(selectionSquare.data(),
                                                                      sizeof(float) * selectionSquare.size());

    auto vertexArraySelectionSquare = std::make_shared<VertexArray>();

    auto indexBufferSelectionSquare = std::make_shared<IndexBuffer>(indicesSelectionSquare.data(), indicesSelectionSquare.size());

    vertexBufferSelectionSquare->SetLayout(*gridBufferLayout);
    vertexArraySelectionSquare->AddVertexBuffer(vertexBufferSelectionSquare);

    vertexArraySelectionSquare->SetIndexBuffer(indexBufferSelectionSquare);
    vertexArraySelectionSquare->Bind();

    for (auto color : arrayColor) {
        unsigned x_coordinate = color[2]==1 ? numberOfSquare - 1 : 0;
        unsigned y_coordinate = 0;
        for (int i = 0; i < numberOfSquare * 2; i++) {
            auto cube = createSelectionCube(color[0], color[1], color[2], x_coordinate, y_coordinate);
            auto vertexArrayCube = std::make_shared<VertexArray>();

            auto indicesCube = GeometricTools::CubeTopology;

            auto indexBufferCube = std::make_shared<IndexBuffer>(indicesCube.data(), indicesCube.size());

            auto vertexBufferCube = std::make_shared<VertexBuffer>(cube.data(), sizeof(float) * cube.size());

            vertexBufferCube->SetLayout(*gridBufferLayout);
            vertexArrayCube->AddVertexBuffer(vertexBufferCube);
            vertexArrayCube->SetIndexBuffer(indexBufferCube);
            vertexArrayCube->Bind();
            cubes[x_coordinate * numberOfSquare + y_coordinate] = vertexArrayCube;
            cubesColor[x_coordinate * numberOfSquare + y_coordinate] = color[2]==1;
            y_coordinate++;
            if(y_coordinate>=numberOfSquare){
                if(x_coordinate > 2) {
                    y_coordinate = 0;
                    x_coordinate--;
                }else {
                    y_coordinate = 0;
                    x_coordinate++;
                }
            }
        }
    }

    vertexBuffer->SetLayout(*gridBufferLayout);
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    vertexArray->Bind();

    //
    // Shader module
    //

    auto* shaderCube = new Shader(vertexShaderSrc, fragmentShaderSrc);
    shaderCube->Bind();

    auto* shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
    shader->Bind();

    // Use PerspectiveCamera class instead
    PerspectiveCamera camera = PerspectiveCamera(PerspectiveCamera::Frustrum{glm::radians(45.0f), 1.0f, 1.0f, 1.0f, -10.0f},
                                                 glm::vec3(0.0f, -3.0f, 2.0f),
                                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                                 glm::vec3(0.0f, 1.0f, 0.0f));

    shader->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());

    shader->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());

    shader->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());

    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0);
    shader->UploadUniformFloat4("u_Color", color);

    glfwSetKeyCallback(window, AssignementApplication::key_callback);

    glEnable(GL_MULTISAMPLE);  // Enabled Multisample
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    //Wireframe mode
    //RenderCommands::SetWireframeMode();

    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // Set clear color to a shade of gray
        RenderCommands::Clear();

        shader->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());

        // Update the vertex buffer with the new data for the selection square using the currentXSelected and currentYSelected

        RenderCommands::DrawIndex(
            GL_TRIANGLES,
            vertexArray
        );
        if (hasMoved) {
            vertexBufferSelectionSquare->BufferSubData(0, sizeof(float) * selectionSquare.size(), createSelectionSquare().data());
            hasMoved = false;
        }
        if(hasCubeSelected) {
            updateSelectedCube();
            hasCubeSelected = false;
        }

        for(auto cube : cubes) {
            RenderCommands::DrawIndex(
                    GL_TRIANGLES,
                    cube.second
            );
        }

        RenderCommands::DrawIndex(
                GL_TRIANGLES,
                vertexArraySelectionSquare
        );

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup of Shader and Buffers
    shader->Unbind();
    vertexArray->Unbind();
    vertexArraySelectionSquare->Unbind();
    for(auto cube : cubes) {
        cube.second->Unbind();
    }

    return stop();
}

unsigned AssignementApplication::stop() {
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

AssignementApplication* AssignementApplication::getAssignementApplication() {
    return AssignementApplication::current_application;
}

void AssignementApplication::exit() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);

}
