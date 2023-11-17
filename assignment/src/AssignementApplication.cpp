#include "AssignementApplication.h"
// shader objects
#include "shaders/grid.h"
#include "shaders/square.h"
#include "shaders/unit.h"
// rendering framework
#include "./../GeometricTools/GeometricTools.h"
#include "../../framework/Rendering/IndexBuffer.h"
#include "./../Rendering/VertexBuffer.h"
#include "VertextArray.h"
#include "../../framework/Rendering/Shader.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "./../Rendering/TextureManager.h"
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

    toggleTexture = false;

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
        // toggle texturing/blending
        case GLFW_KEY_T:
            getAssignementApplication()->setTextureState();
            break;
        default:
            break;
        }
    }
}

void AssignementApplication::setTextureState() {
    toggleTexture = !toggleTexture;
    std::cout << toggleTexture << std::endl;
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
std::vector<float> AssignementApplication::createSquare(float opacity) const {

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

    auto squareVertices = createSquare(1);
    auto squareIndices = GeometricTools::TopologySquare2D;

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
    auto squareLayout = std::make_shared<BufferLayout>(BufferLayout({
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
    auto VAO_Square = std::make_shared<VertexArray>();
    VAO_Square->Bind(); 
    auto VBO_Square = std::make_shared<VertexBuffer>(squareVertices.data(),
        sizeof(float) * squareVertices.size()); 
    VBO_Square->SetLayout(*squareLayout); 
    VAO_Square->AddVertexBuffer(VBO_Square); 
    auto IBO_Square = std::make_shared<IndexBuffer>(squareIndices.data(), 
        static_cast<GLsizei>(squareIndices.size()));
    VAO_Square->SetIndexBuffer(IBO_Square);

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


    //--------------------------------------------------------------------------------------------------------------
    //
    // Shader module
    //
    //--------------------------------------------------------------------------------------------------------------
    // grid shader
    auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
    shaderGrid->Bind();
    // square shader
    auto* shaderSquare = new Shader(VS_Square, FS_Square);
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
    GLuint unitTextureUnit = textureManager->GetUnitByName("cubeTexture"); 


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Renderloop variables
    glm::mat4 unitModel = glm::mat4(1.0f);
    glm::mat4 squareModel = glm::mat4(1.0f);
    hasMoved = true;
    bool setup = 1;

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

        //--------------------------------------------------------------------------------------------------------------
        //
        // grid processing
        //
        //--------------------------------------------------------------------------------------------------------------
        VAO_Grid->Bind();
        shaderGrid->Bind();
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderUnit->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
        shaderGrid->UploadUniform1i("uTexture", gridTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid); 
        
        //--------------------------------------------------------------------------------------------------------------
        //
        // unit processing
        //
        // changes to the square only in the first iteration
        //--------------------------------------------------------------------------------------------------------------
        if (setup)
        {
            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare);
            float bottomRight = numberOfSquare * sideLength - 1;

            // transforming and scaling units
            unitModel = glm::mat4(1.0f);
            float translationX = bottomRight - sideLength / 2;
            float translationY = bottomRight + sideLength / 2 - sideLength * numberOfSquare;
            unitModel = glm::translate(unitModel, glm::vec3(translationX, translationY, sideLength / 2));
            float scaleValue = 0.6f;
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, 0.7, scaleValue));
            unitModel *= scaleMatrix;
            
            setup = false;
        }

        // bind Unit Buffers, upload unit uniforms and draw unit
        // [0,1]
        float unitOpacity = 1;
        // team color
        glm::vec3 teamRed = glm::vec3(1.0, 0.0, 0.0);
        glm::vec3 teamBlue = glm::vec3(0.0, 0.0, 1.0);
        glm::vec3 uploadingColor = teamRed;

        VAO_Unit->Bind();
        shaderUnit->Bind();
        shaderUnit->UploadUniformMatrix4fv("u_Model", unitModel);
        shaderUnit->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderUnit->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderUnit->UploadUniformFloat1("u_TextureState", static_cast<float>(toggleTexture));
        shaderUnit->UploadUniformFloat1("u_Opacity", unitOpacity); 
        shaderUnit->UploadUniformFloat3("u_Color", uploadingColor);
        shaderUnit->UploadUniform1i("CubeMap", unitTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Unit);

        //--------------------------------------------------------------------------------------------------------------
        //
        // square processing
        //
        //--------------------------------------------------------------------------------------------------------------
        if (hasMoved) {

            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare);
            float bottomRight = numberOfSquare * sideLength - 1;
            
            // translating square
            glm::mat4 squareModel = glm::mat4(1.0f);
            float translationX = bottomRight - currentXSelected * sideLength - sideLength / 2;
            float translationY = bottomRight + currentYSelected * sideLength + sideLength / 2 - sideLength * numberOfSquare;
            squareModel = glm::translate(squareModel, glm::vec3(translationX, translationY, sideLength / 2 + 0.01f));

            hasMoved = false;
        }

        // bind square buffer, upload square uniforms and draw square
        VAO_Square->Bind();
        shaderSquare->Bind();
        shaderSquare->UploadUniformMatrix4fv("u_Model", squareModel);
        shaderSquare->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderSquare->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Square);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //--------------------------------------------------------------------------------------------------------------
    //
    // OpenGL cleanup
    //
    //--------------------------------------------------------------------------------------------------------------

    // Cleanup Grid Buffers
    shaderGrid->~Shader();
    VAO_Grid->~VertexArray();
    VBO_Grid->~VertexBuffer();
    IBO_Grid->~IndexBuffer();

    // Cleanup Square Buffers
    shaderSquare->~Shader();
    VAO_Square->~VertexArray();
    VBO_Square->~VertexBuffer();
    IBO_Square->~IndexBuffer();

    // Cleanup Unit Buffers
    shaderUnit->~Shader();
    VAO_Unit->~VertexArray();
    VBO_Unit->~VertexBuffer();
    IBO_Unit->~IndexBuffer();

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
