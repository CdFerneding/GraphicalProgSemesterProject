#include "Lab4Application.h"
#include "shaderGrid.h"
#include "shaderCube.h"
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
#include "./../Rendering/TextureManager.h"
#include <RenderCommands.h>

// stb requires the use of a compilation definition
#define STB_IMAGE_IMPLEMENTATION


Lab4Application* Lab4Application::current_application = nullptr;

Lab4Application::Lab4Application(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {
    currentXSelected = 0;
    currentYSelected = 0;
    rotationAngleX = 0;
    rotationAngleY = 0;
    currentRotationAngleX = 0;
    currentRotationAngleY = 0;
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

         // There is two key for up and left because we are using a QWERTY and an AZERTY keyboard
        case GLFW_KEY_Z:
        case GLFW_KEY_W:
            getLab4Application()->rotateCube(UP);
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_Q:
            getLab4Application()->rotateCube(LEFT);
            break;
        case GLFW_KEY_S:
            getLab4Application()->rotateCube(DOWN);
            break;
        case GLFW_KEY_D:
            getLab4Application()->rotateCube(RIGHT);
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

void Lab4Application::rotateCube(Direction direction) {
    hasRotated = true;
    switch (direction) {
    case UP:
        rotationAngleX = ((int)rotationAngleX + 5) % 360;
        break;
    case DOWN:
        rotationAngleX = ((int)rotationAngleX - 5) % 360;
        break;
    case LEFT:
        rotationAngleY = ((int)rotationAngleY + 5) % 360;
        break;
    case RIGHT:
        rotationAngleY = ((int)rotationAngleY - 5) % 360;
        break;
    default:
        break;
    }
}

std::vector<float> Lab4Application::createSelectionCube() const {
    float cubeSize = 2.0f / static_cast<float>(numberOfSquare);
    float XSelected = static_cast<float>(currentXSelected);
    float YSelected = static_cast<float>(currentYSelected);

    std::vector<float> selectionCube = {
        1 - cubeSize * XSelected, -1 + cubeSize * YSelected, 0.1, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * XSelected, -1 + cubeSize * (YSelected + 1), 0.1, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * (XSelected + 1), -1 + cubeSize * (YSelected + 1), 0.1, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * (XSelected + 1), -1 + cubeSize * YSelected, 0.1, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * XSelected, -1 + cubeSize * YSelected, cubeSize, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1, 
        1 - cubeSize * XSelected, -1 + cubeSize * (YSelected + 1), cubeSize, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * (XSelected + 1), -1 + cubeSize * (YSelected + 1), cubeSize, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
        1 - cubeSize * (XSelected + 1), -1 + cubeSize * YSelected, cubeSize, 185 / 255.0f, 89 / 255.0f, 235 / 255.0f, 1,
    };
    return selectionCube;
}


unsigned Lab4Application::Run() {
    current_application = this;
    hasRotated = false;
    hasMoved = false;


    //--------------------------------------------------------------------------------------------------------------
    //
    //  define vertices and indices for the grid and the cube
    //
    //--------------------------------------------------------------------------------------------------------------


    auto gridVertices = GeometricTools::UnitGridGeometry2DWTCoords(numberOfSquare);
    auto gridIndices = GeometricTools::UnitGrid2DTopology(numberOfSquare); 

    auto cubeVertices = createSelectionCube();
    auto cubeIndices = GeometricTools::CubeTopology; 
    
    
    //--------------------------------------------------------------------------------------------------------------
    //
    //  define the layout for the grid and the cube
    //
    //--------------------------------------------------------------------------------------------------------------

    // grid Layout
    auto gridLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        {ShaderDataType::Float2, "texCoords", false}
        }));
    // cube Layout
    auto cubeLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false},
        {ShaderDataType::Float4, "color", false},
        }));

    // calculate numberOfAttributes of the cube to dynamically handle moving and rotation of the cube
    unsigned int numOfAttributes = 0;
    for (int i = 0; i < cubeLayout->GetAttributes().size(); i++) {
        // considering we are using Float: division by 4
        // otherwise I think a switch case statement would be necessary 
        auto attribute = cubeLayout->GetAttributes()[i];
        numOfAttributes += attribute.Size / 4;
    }

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

    // VAO Cube
    auto VAO_Cube = std::make_shared<VertexArray>();
    VAO_Cube->Bind();
    auto VBO_Cube = std::make_shared<VertexBuffer>(cubeVertices.data(), sizeof(float) * cubeVertices.size()); 
    VBO_Cube->SetLayout(*cubeLayout);
    VAO_Cube->AddVertexBuffer(VBO_Cube);
    auto IBO_Cube = std::make_shared<IndexBuffer>(cubeIndices.data(), static_cast<GLsizei>(cubeIndices.size()));
    VAO_Cube->SetIndexBuffer(IBO_Cube);


    //--------------------------------------------------------------------------------------------------------------
    //
    // Shader module
    //
    //--------------------------------------------------------------------------------------------------------------
    // Grid shader
    auto* shaderGrid = new Shader(VS_Grid, FS_Grid);
    shaderGrid->Bind();
    // Cube
    auto* shaderCube = new Shader(VS_Cube, FS_Cube);
    shaderCube->Bind();


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
    bool success2D = textureManager->LoadTexture2DRGBA("gridTexture", "resources/textures/black-tile.jpg", 0, true);
    if (!success2D) {
        std::cout << "2D Texture not loaded correctly." << std::endl;
    }
    // Give the textures to the shader
    GLuint gridTextureUnit = textureManager->GetUnitByName("gridTexture"); 

    // Load Cube Map
    bool successCube = textureManager->LoadCubeMapRGBA("cubeTexture", "resources/textures/black-tile.jpg", 0, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint cubeTextureUnit = textureManager->GetUnitByName("cubeTexture");
    
    glfwSetKeyCallback(window, Lab4Application::key_callback);

    // Enable blending
    glEnable(GL_BLEND);
    // Set the blending function: s*alpha + d(1-alpha)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    //Wireframe mode
    //RenderCommands::SetWireframeMode();


    //--------------------------------------------------------------------------------------------------------------
    //
    // start execution
    //
    //--------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        //preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // Set clear color to a shade of gray
        RenderCommands::Clear();

        // input
        glfwSetKeyCallback(window, Lab4Application::key_callback); 

        // draw Grid
        VAO_Grid->Bind();
        shaderGrid->Bind(); 
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderGrid->UploadUniform1i("uTexture", gridTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid);
        
        // Bind Cube Buffers
        VAO_Cube->Bind();
        shaderCube->Bind(); 


        if (hasMoved) {
            cubeVertices = createSelectionCube();
            //Update vertexbufferCube with the content of createSelectionCube
            VBO_Cube->BufferSubData(0, sizeof(float) * cubeVertices.size(), cubeVertices.data());

            hasMoved = false;
            hasRotated = true;
            currentRotationAngleX = 0;
            currentRotationAngleY = 0;
        }

        if (hasRotated) {
            // Get the gravity center of the cube
            float gravityPoint[3] = { 0.0f, 0.0f, 0.0f };

            for (int i = 0; i < cubeVertices.size(); i += numOfAttributes) {
                gravityPoint[0] += cubeVertices[i];
                gravityPoint[1] += cubeVertices[i + 1];
                gravityPoint[2] += cubeVertices[i + 2];
            }

            gravityPoint[0] /= 8;
            gravityPoint[1] /= 8;
            gravityPoint[2] /= 8;

            //To rotate the cube around its center, we need to translate the cube to the origin, rotate it and translate it back to its original position
            cubeVertices = GeometricTools::translateCubeGeneric(cubeVertices, -gravityPoint[0], -gravityPoint[1], -gravityPoint[2], numOfAttributes);

            //Rotate the cube
            cubeVertices = GeometricTools::rotateCubeGeneric(cubeVertices, rotationAngleX - currentRotationAngleX, rotationAngleY - currentRotationAngleY, 0, numOfAttributes);

            //Translate the cube back to its original position
            cubeVertices = GeometricTools::translateCubeGeneric(cubeVertices, gravityPoint[0], gravityPoint[1], gravityPoint[2], numOfAttributes);

            //Update vertexbufferCube with the content of new_cube
            VBO_Cube->BufferSubData(0, sizeof(float) * 5 * 8, cubeVertices.data());

            currentRotationAngleX = rotationAngleX;
            currentRotationAngleY = rotationAngleY;

            hasRotated = false;
        }

        // give uniforms, draw cube
        shaderCube->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix()); 
        shaderCube->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix()); 
        shaderCube->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix()); 
        shaderCube->UploadUniform1i("CubeMap", cubeTextureUnit); 
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Cube); 

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
