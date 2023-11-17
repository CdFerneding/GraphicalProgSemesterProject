#include "AssignementApplication.h"
// shader objects
#include "shaders/grid.h"
#include "shaders/square.h"
#include "shaders/cube.h"
// rendering framework
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include <RenderCommands.h>
// libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

AssignementApplication* AssignementApplication::current_application = nullptr;

AssignementApplication::AssignementApplication(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {

    currentXSelected = 0;
    currentYSelected = 0;
    currentCubeSelected = -1;

    hasMoved = false;
    hasCameraChanged = false;
    hasCubeSelected = false;

    previousPosition = {-1, -1};
    previousPositionSelector = {0, 0};

    shader = nullptr;

    for(int i=0; i < numberOfSquare; i++) {
        for(int j=0; j < numberOfSquare; j++) {
            vertexArrayIdPerCoordinate[i][j] = -1;
        }
    }
}

AssignementApplication::~AssignementApplication() = default;


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
/*
void AssignementApplication::moveCubeRequest() {

    bool isNotEmpty = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] != -1;
    std::array<int, 2> new_coordinate = {static_cast<int>(currentXSelected), static_cast<int>(currentYSelected)};

    if (currentCubeSelected != -1) {

        bool colorSelectedCube = colorVertexArrays[vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]]];

        if(new_coordinate[0] == previousPosition[0] && new_coordinate[1] == previousPosition[1]) {

            new_coordinate = previousPosition;

            // In case we plan to use multiple vertexBuffer, we need to make sure
            // the one with the coordinate and color is in the first position

            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(0);

            auto cube = createUnit();

            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            VBO_SelectionSquare->BufferSubData(
                    0, sizeof(float) * selectionSquare.size(),
                    createSelectionSquare(opacity).data());
        }
        else if (isNotEmpty) {

            new_coordinate = previousPosition;

            // In case we plan to use multiple vertexBuffer, we need to make sure
            // the one with the coordinate and color is in the first position

            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(0);

            auto cube = createUnit();

            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
        }else {

            // In case we plan to use multiple vertexBuffer, we need to make sure
            // the one with the coordinate and color is in the first position
            auto vertexBuffer = currentSelectedVertexArray->getVertexBuffer(0);
            auto cube = createUnit();
            vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

            vertexArrayIdPerCoordinate[new_coordinate[0]][new_coordinate[1]] =
                    vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]];

            vertexArrayIdPerCoordinate[previousPosition[0]][previousPosition[1]] = -1;

            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            VBO_SelectionSquare->BufferSubData(
                    0, sizeof(float) * selectionSquare.size(),
                    createSelectionSquare(opacity).data());

        }
        previousPosition = {static_cast<int>(-1), static_cast<int>(-1)};
        currentCubeSelected = -1;
        currentSelectedVertexArray = nullptr;
    }else if (isNotEmpty) {

        auto current_cube = vertexArrays[vertexArrayIdPerCoordinate[currentXSelected][currentYSelected]];
        currentCubeSelected = currentXSelected * numberOfSquare + currentYSelected;

        auto vertexBuffer = current_cube->getVertexBuffer(0);
        auto cube = createUnit();
        vertexBuffer->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

        currentSelectedVertexArray = current_cube;
        previousPosition = new_coordinate;
    }
}
*/
std::vector<float> AssignementApplication::createSelectionSquare(float opacity) const {

    std::vector<float> newSelectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.0001, 0, 1, 0, opacity,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0001, 0, 1, 0, opacity
    };

    return newSelectionSquare;
}

std::vector<float> AssignementApplication::createUnit() const {
    float sideLength = 2.0f / static_cast<float>(numberOfSquare);
    float halfSideLength = sideLength * 0.5f;

    // cube centered around origin in relation to the gridSquareSize
    std::vector<float> selectionCube = {
        -halfSideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, -halfSideLength + sideLength, -halfSideLength,
        -halfSideLength + sideLength, -halfSideLength + sideLength, -halfSideLength,
        -halfSideLength + sideLength, -halfSideLength, -halfSideLength,
        -halfSideLength, -halfSideLength, halfSideLength,
        -halfSideLength, -halfSideLength + sideLength, halfSideLength,
        -halfSideLength + sideLength, -halfSideLength + sideLength, halfSideLength,
        -halfSideLength + sideLength, -halfSideLength, halfSideLength,
    };
    return selectionCube;
}


unsigned AssignementApplication::Run() {

    current_application = this;
    hasCameraChanged = false;
    hasMoved = false;

    //--------------------------------------------------------------------------------------------------------------
    //
    //  define vertices and indices for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------
    auto gridVertices = GeometricTools::UnitGridGeometry2DWTCoords(numberOfSquare);
    auto gridIndices = GeometricTools::UnitGrid2DTopology(numberOfSquare);

    auto selectionSquareVertices = createSelectionSquare(0);
    auto selectionSquareIndices = GeometricTools::TopologySquare2D;

    auto unitVertices = createUnit();
    auto unitIndices = GeometricTools::CubeTopology;


    //--------------------------------------------------------------------------------------------------------------
    //
    //  define the layout for the grid, selectionSquare and the unit
    //
    //--------------------------------------------------------------------------------------------------------------

    // grid Layout
    auto gridLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        {ShaderDataType::Float2, "texCoords", false}
        }));

    // selectionSquareLayout
    auto selectionSquareLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false}, 
        {ShaderDataType::Float4, "color", false},
        }));

    // cube Layout
    auto unitLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        }));

    //--------------------------------------------------------------------------------------------------------------
    //
    //  prepping for the grid and the cube
    //
    //--------------------------------------------------------------------------------------------------------------

    // VAO Grid
    auto VAO_Grid = std::make_shared<VertexArray>();
    VAO_Grid->Bind();
    auto VBO_Grid = std::make_shared<VertexBuffer>(gridVertices.data(), sizeof(float) * gridVertices.size());
    VBO_Grid->SetLayout(*gridLayout);
    VAO_Grid->AddVertexBuffer(VBO_Grid);
    auto IBO_Grid = std::make_shared<IndexBuffer>(gridIndices.data(), static_cast<GLsizei>(gridIndices.size()));
    VAO_Grid->SetIndexBuffer(IBO_Grid); 

    // VAO SelectionSquare
    auto VAO_SelectionSquare = std::make_shared<VertexArray>();
    VAO_SelectionSquare->Bind(); 
    auto VBO_SelectionSquare = std::make_shared<VertexBuffer>(selectionSquareVertices.data(),
        sizeof(float) * selectionSquareVertices.size()); 
    VBO_SelectionSquare->SetLayout(*selectionSquareLayout); 
    VAO_SelectionSquare->AddVertexBuffer(VBO_SelectionSquare); 
    auto IBO_SelectionSquare = std::make_shared<IndexBuffer>(selectionSquareIndices.data(), 
        static_cast<GLsizei>(selectionSquareIndices.size()));
    VAO_SelectionSquare->SetIndexBuffer(IBO_SelectionSquare);

    // VAO Unit
    auto VAO_Unit = std::make_shared<VertexArray>();
    VAO_Unit->Bind();
    auto VBO_Unit = std::make_shared<VertexBuffer>(unitVertices.data(),
        sizeof(float) * unitVertices.size());
    VBO_Unit->SetLayout(*unitLayout);
    VAO_Unit->AddVertexBuffer(VBO_Unit);
    auto IBO_Unit = std::make_shared<IndexBuffer>(unitIndices.data(),
        static_cast<GLsizei>(unitIndices.size()));
    VAO_Unit->SetIndexBuffer(IBO_Unit);
    /*
    for(int i=0; i < numberOfSquare * numberOfSquare; i++) {
        cubes[i] = nullptr;
    }

    int count = 0;

    std::vector<std::vector<float>> arrayColor = {
            {1.0f, 0.0f, 0.0f}, //red
            {0.0f, 0.0f, 1.0f} //blue
    };

    // For each of the 2 type of color (red and blue)
    for (auto color : arrayColor) {

        unsigned x_coordinate = color[2]==1 ? numberOfSquare - 1 : 0;
        unsigned y_coordinate = 0;

        for (int i = 0; i < numberOfSquare * 2; i++) {

            
            // Fix when you launch the program so the selected cube is green
             
                std::vector<float> cube;

                if (color[0] == arrayColor[0][0] && i == 0) {
                    cube = createUnit(0, 1, 0, x_coordinate, y_coordinate);
                } else {
                    cube = createUnit(color[0], color[1], color[2], x_coordinate, y_coordinate);
                }


            auto vertexArrayCube = std::make_shared<VertexArray>();

            auto indicesCube = GeometricTools::CubeTopology;

            auto indexBufferCube = std::make_shared<IndexBuffer>(
                    indicesCube.data(), indicesCube.size());

            auto vertexBufferCube = std::make_shared<VertexBuffer>(
                    cube.data(), sizeof(float) * cube.size());

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
    */

    //--------------------------------------------------------------------------------------------------------------
    //
    // Shader module
    //
    //--------------------------------------------------------------------------------------------------------------
    // grid shader
    auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
    shaderGrid->Bind();
    // square shader
    auto* shaderSquare = newShader(VS_Square, FS_Square);
    shaderSquare->Bind();
    // cube shader
    auto* shaderUnit = new Shader(VS_Unit, FS_Unit);
    shaderUnit->Bind();


    //--------------------------------------------------------------------------------------------------------------
    //
    // camera
    //
    //--------------------------------------------------------------------------------------------------------------
    // Use PerspectiveCamera class instead
    PerspectiveCamera camera = PerspectiveCamera( 
        PerspectiveCamera::Frustrum{ glm::radians(45.0f), 1.0f, 1.0f, 1.0f, -10.0f }, // frustrum
        glm::vec3(0.0f, -3.0f, 2.0f), // camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // lookAt
        glm::vec3(0.0f, 1.0f, 0.0f)); // upVector 

    //--------------------------------------------------------------------------------------------------------------
    //
    // Texture module
    //
    //--------------------------------------------------------------------------------------------------------------
    TextureManager* textureManager = TextureManager::GetInstance(); 
    // Load 2D texture for the grid
    bool success2D = textureManager->LoadTexture2DRGBA("gridTexture", "resources/textures/floor_texture.jpg", 0, true);
    if (!success2D) {
        std::cout << "2D Texture not loaded correctly." << std::endl;
    }
    // Give the textures to the shader
    GLuint gridTextureUnit = textureManager->GetUnitByName("gridTexture"); 

    // Load Cube Map
    //TODO: loading the wood texture (all textures exept black-tile) does not work for the cubemap (whyever that might be).
    bool successCube = textureManager->LoadCubeMapRGBA("cubeTexture", "resources/textures/cube_texture.jpg", 0, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint cubeTextureUnit = textureManager->GetUnitByName("cubeTexture"); 


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Renderloop variables
    glm::mat4 model = glm::mat4(1.0f);
    hasMoved = true;

    //--------------------------------------------------------------------------------------------------------------
    //
    // start execution
    //
    // important notice:
    // for semi-transparancy (blending) to work opaque objects have to be drawn first!
    // Also OpenGL Depth-Test functionality might interfere
    //--------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {

        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // grey background
        RenderCommands::Clear();

        // handle input
        glfwSetKeyCallback(window, AssignementApplication::key_callback);

        // bind grid buffer, upload grid uniforms, draw grid
        VAO_Grid->Bind();
        shaderGrid->Bind();
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderGrid->UploadUniform1i("uTexture", gridTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid); 

        /*
        if (hasMoved) {

            float opacity = vertexArrayIdPerCoordinate[currentXSelected][currentYSelected] == -1 ? 1 : 0;
            VBO_SelectionSquare->BufferSubData(0, sizeof(float) * selectionSquareVertices.size(),
                                                       createSelectionSquare(opacity).data());

            if(opacity == 0 && !(previousPosition[0] == currentXSelected && previousPosition[1] == currentYSelected)) {

                //Update the current cube and change his color to green

                auto current_cube = vertexArrays[vertexArrayIdPerCoordinate[currentXSelected][currentYSelected]];
                auto vertexBufferSelected = current_cube->getVertexBuffer(0);

                auto cube = createUnit(0.0f, 1.0f, 0.0f, currentXSelected, currentYSelected);
                vertexBufferSelected->BufferSubData(0, sizeof(float) * cube.size(), cube.data());

            }

            if(vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]] != -1 &&
                !(previousPositionSelector[0] == previousPosition[0] && previousPosition[1] == previousPositionSelector[1]))
            {

                auto previous_cube = vertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]];
                auto vertexBufferSelected = previous_cube->getVertexBuffer(0);

                float r = colorVertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]] ? 0 : 1;
                float b = colorVertexArrays[vertexArrayIdPerCoordinate[previousPositionSelector[0]][previousPositionSelector[1]]] ? 1 : 0;

                auto cube = createUnit(r, 0.0f, b, previousPositionSelector[0], previousPositionSelector[1]);
                vertexBufferSelected->BufferSubData(0, sizeof(float) * cube.size(), cube.data());
            }

            previousPositionSelector[0] = currentXSelected;
            previousPositionSelector[1] = currentYSelected;
            hasMoved = false;
        }

        if(hasCubeSelected) {
            moveCubeRequest();
            hasCubeSelected = false;
        }

        if(hasCameraChanged) {
            shader->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
            shader->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
            shader->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
            hasCameraChanged = false;
        }

        for(const auto& cube : cubes) {
            if(cube!=nullptr)
                RenderCommands::DrawIndex(
                        GL_TRIANGLES,
                        cube
                );
        }
        */

        // bind Square Buffers and draw square
        VAO_Unit->Bind();
        shaderUnit->Bind();
        shaderUnit->UploadUniformMatrix4fv("u_Model", model);
        shaderUnit->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderUnit->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderUnit->UploadUniformFloat4("u_CubeColor", cubeColor);
        shaderUnit->UploadUniform1i("CubeMap", cubeTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Unit);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup Grid Buffers
    shaderGrid->~Shader();
    VAO_Grid->~VertexArray();
    VBO_Grid->~VertexBuffer();
    IBO_Grid->~IndexBuffer();

    // Cleanup Cube Buffers
    shaderCube->~Shader();
    VAO_Cube->~VertexArray();
    VBO_Grid->~VertexBuffer();
    IBO_Cube->~IndexBuffer();

    // Cleanup Square Buffers

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
