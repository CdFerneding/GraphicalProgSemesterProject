#include "AssignementApplication.h"
#include "shaderAssignement.h"
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
    //fill vertexArrayIdPerCoordinate with -1
    for(int i=0; i < numberOfSquare; i++) {
        for(int j=0; j < numberOfSquare; j++) {
            vertexArrayIdPerCoordinate[i][j] = -1;
        }
    }
}

AssignementApplication::~AssignementApplication() {

}
void AssignementApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
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
		    getAssignementApplication()->move(UP);
		    break;
        case GLFW_KEY_A:
            getAssignementApplication()->move(LEFT);
            break;
        case GLFW_KEY_Q:
            getAssignementApplication()->exit();
            break;
        case GLFW_KEY_S:
            getAssignementApplication()->move(DOWN);
            break;
        case GLFW_KEY_D:
            getAssignementApplication()->move(RIGHT);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_ENTER:
            getAssignementApplication()->select();
            break;
        case GLFW_KEY_P:
            getAssignementApplication()->zoom(1.0f);
            break;
        case GLFW_KEY_O:
            getAssignementApplication()->zoom(-1.0f);
            break;
        case GLFW_KEY_L:
            getAssignementApplication()->rotate(5);
            break;
        case GLFW_KEY_H:
            getAssignementApplication()->rotate(-5);
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
    camera.rotateArroundLookAt(degree);
    hasCameraChanged = true;
}

void AssignementApplication::zoom(float zoomValue) {
    camera.zoom(zoomValue);
    hasCameraChanged = true;
}

void AssignementApplication::select() {
     hasCubeSelected = true;
}

void AssignementApplication::updateSelectedCube() {
    auto isNotEmpty = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] != -1;
    std::array<int, 2> new_coordinate = {static_cast<int>(currentXSelected), static_cast<int>(currentYSelected)};
    if (currentCubeSelected != -1) {
        bool colorSelectedCube = colorVertexArrays[vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]]];
        if(new_coordinate[0] == previousPosition[0] && new_coordinate[1] == previousPosition[1]) {
            new_coordinate = previousPosition;
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(currentSelectedVertexArray->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(0, 1, 0, new_coordinate[0], new_coordinate[1]);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            vertexBufferSelectionSquare->BufferSubData(0, sizeof(float) * selectionSquare.size(), createSelectionSquare(opacity).data());
        }
        else if (isNotEmpty) {
            new_coordinate = previousPosition;
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(currentSelectedVertexArray->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(colorSelectedCube ? 0 : 1, 0.0f, colorSelectedCube ? 1 : 0, new_coordinate[0], new_coordinate[1]);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
        }else {
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(
                    currentSelectedVertexArray->getNumberOfVertexBuffers() -
                    1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(0.0f, 1.0f, 0.0f,
                                            new_coordinate[0], new_coordinate[1]);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

            vertexArrayIdPerCoordinate[new_coordinate[0]][new_coordinate[1]] = vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]];
            vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]] = -1;
            cubeJustPlaced = true;
            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            vertexBufferSelectionSquare->BufferSubData(0, sizeof(float) * selectionSquare.size(), createSelectionSquare(opacity).data());
        }
        previousPosition = {static_cast<int>(-1), static_cast<int>(-1)};
        currentCubeSelected = -1;
        currentSelectedVertexArray = nullptr;
    }else {
        if (isNotEmpty) {
            auto current_cube = vertexArrays[vertexArrayIdPerCoordinate[currentXSelected][currentYSelected]];
            currentCubeSelected = currentXSelected * numberOfSquare + currentYSelected;
            auto vertexBuffer = current_cube->getVertexBuffer(current_cube->getNumberOfVertexBuffers() - 1); // In case we plan to use multiple vertexBuffer, we need to make sure the one with the coordinate and color is in the last position
            auto cube = createSelectionCube(1.0f, 1.0f, 0.0f, currentXSelected, currentYSelected);
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            currentSelectedVertexArray = current_cube;
            previousPosition = new_coordinate;
            hasJustSelected = true;
        }
    }
}

std::vector<float> AssignementApplication::createSelectionSquare(float opacity) const {

    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, opacity
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

    selectionSquare = createSelectionSquare(0);

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

    vertexBufferSelectionSquare = std::make_shared<VertexBuffer>(selectionSquare.data(),
                                                                      sizeof(float) * selectionSquare.size());

    auto vertexArraySelectionSquare = std::make_shared<VertexArray>();

    auto indexBufferSelectionSquare = std::make_shared<IndexBuffer>(indicesSelectionSquare.data(), indicesSelectionSquare.size());

    vertexBufferSelectionSquare->SetLayout(*gridBufferLayout);
    vertexArraySelectionSquare->AddVertexBuffer(vertexBufferSelectionSquare);

    vertexArraySelectionSquare->SetIndexBuffer(indexBufferSelectionSquare);
    vertexArraySelectionSquare->Bind();

    for(int i=0; i < numberOfSquare * numberOfSquare; i++) {
        cubes[i] = nullptr;
    }
    unsigned int count = 0;
    for (auto color : arrayColor) {
        unsigned x_coordinate = color[2]==1 ? numberOfSquare - 1 : 0;
        unsigned y_coordinate = 0;
        for (int i = 0; i < numberOfSquare * 2; i++) {
            //Fix when you launch the program so the selected cube is green
            std::vector<float> cube;
            if(color[0] == arrayColor[0][0] && i == 0) {
                cube = createSelectionCube(0, 1, 0, x_coordinate, y_coordinate);
            }else {
                cube = createSelectionCube(color[0], color[1], color[2], x_coordinate, y_coordinate);
            }
            auto vertexArrayCube = std::make_shared<VertexArray>();

            auto indicesCube = GeometricTools::CubeTopology;

            auto indexBufferCube = std::make_shared<IndexBuffer>(indicesCube.data(), indicesCube.size());

            auto vertexBufferCube = std::make_shared<VertexBuffer>(cube.data(), sizeof(float) * cube.size());

            vertexBufferCube->SetLayout(*gridBufferLayout);
            vertexArrayCube->AddVertexBuffer(vertexBufferCube);
            vertexArrayCube->SetIndexBuffer(indexBufferCube);
            vertexArrayCube->Bind();
            cubes[x_coordinate * numberOfSquare + y_coordinate] = vertexArrayCube;

            vertexArrayIdPerCoordinate[x_coordinate][y_coordinate] = count++;
            vertexArrays.push_back(vertexArrayCube);
            colorVertexArrays.push_back(color[2]==1);

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

    shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
    shader->Bind();

    // Use PerspectiveCamera class instead
    camera = PerspectiveCamera(PerspectiveCamera::Frustrum{glm::radians(45.0f), 1.0f, 1.0f, 1.0f, 10.0f},
                                                 glm::vec3(0.0f, -3.0f, 2.0f),
                                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                                 glm::vec3(0.0f, 0.0f, 1.0f));

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
            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            vertexBufferSelectionSquare->BufferSubData(0, sizeof(float) * selectionSquare.size(), createSelectionSquare(opacity).data());

            if(opacity == 0 && !(previousPosition[0] == currentXSelected && previousPosition[1] == currentYSelected)) {
                //Update the current cube and change his color to green
                auto current_cube = vertexArrays[vertexArrayIdPerCoordinate[currentXSelected][currentYSelected]];
                auto vertexBufferSelected = current_cube->getVertexBuffer(0);
                auto cube = createSelectionCube(0.0f, 1.0f, 0.0f, currentXSelected, currentYSelected);
                vertexBufferSelected->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

            }
            if(vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]] != -1 && !(previousPositionSelector[0] == previousPosition[0] && previousPosition[1] == previousPositionSelector[1])) {

                auto previous_cube = vertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]];
                auto vertexBufferSelected = previous_cube->getVertexBuffer(0);
                float r = colorVertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]] ? 0 : 1;
                float b = colorVertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]] ? 1 : 0;
                auto cube = createSelectionCube(r, 0.0f, b, previousPositionSelector[0], previousPositionSelector[1]);
                vertexBufferSelected->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            }
            previousPositionSelector[0] = currentXSelected;
            previousPositionSelector[1] = currentYSelected;
            hasMoved = false;
        }
        if(hasCubeSelected) {
            updateSelectedCube();
            hasCubeSelected = false;
        }
        if(hasCameraChanged) {
            shader->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
            shader->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
            shader->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
            hasCameraChanged = false;
        }
        for(auto cube : cubes) {
            if(cube!=nullptr)
                RenderCommands::DrawIndex(
                        GL_TRIANGLES,
                        cube
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
        cube->Unbind();
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
