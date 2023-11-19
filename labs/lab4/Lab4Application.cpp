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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./../Rendering/TextureManager.h"
#include <RenderCommands.h>

// stb requires the use of a compilation definition
#define STB_IMAGE_IMPLEMENTATION

glm::vec4 Lab4Application::cubeColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
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
        // input for moving the cube
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

        // input for rotation
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

        // input for changing the cube color
        case GLFW_KEY_1:
            cubeColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red
            break;
        case GLFW_KEY_2:
            cubeColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);  // Green
            break;
        case GLFW_KEY_3:
            cubeColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);  // Blue
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
        currentYSelected = (currentYSelected - 1 + numberOfSquare) % numberOfSquare;
        break;
    case LEFT:
        currentXSelected = (currentXSelected + 1) % numberOfSquare;
        break;
    case RIGHT:
        currentXSelected = (currentXSelected - 1 + numberOfSquare) % numberOfSquare;
        break;
    default:
        hasMoved = false;
        break;
    }
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
    float sideLength = 2.0f / static_cast<float>(numberOfSquare);
    float halfSideLength = sideLength * 0.5f;
    sideLength = 1;
    halfSideLength = 0.5;

    // cube centered around origin
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
    // Cube
    auto* shaderCube = new Shader(VS_Cube, FS_Cube);


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
    // texture pictures are: "white-tile", "black-tile", "grass", "wood"
    //--------------------------------------------------------------------------------------------------------------
    TextureManager* textureManager = TextureManager::GetInstance(); 
    // Load 2D texture for the grid
    bool success2D = textureManager->LoadTexture2DRGBA("gridTexture", "resources/textures/white-tile.jpg", 0, true);
    if (!success2D) {
        std::cout << "2D Texture not loaded correctly." << std::endl;
    }
    // Give the textures to the shader
    GLuint gridTextureUnit = textureManager->GetUnitByName("gridTexture"); 

    // Load Cube Map
    bool successCube = textureManager->LoadCubeMapRGBA("cubeTexture", "resources/textures/black-tile.jpg", 1, true);
    if (!successCube) {
        std::cout << "Cube Map not loaded correctly." << std::endl;
    }
    GLuint cubeTextureUnit = textureManager->GetUnitByName("cubeTexture");
    
    glfwSetKeyCallback(window, Lab4Application::key_callback);

    // Enable blending (neccessary for semi-transparent cube)
    glEnable(GL_BLEND);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
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
        // preparation of Window and Shader
        RenderCommands::SetClearColor(0.663f, 0.663f, 0.663f, 1.0f); // Set clear color to a shade of gray
        RenderCommands::Clear();

        // input handling
        glfwSetKeyCallback(window, Lab4Application::key_callback);

        // bind grid buffer, upload grid uniforms, draw grid
        VAO_Grid->Bind();
        shaderGrid->Bind();
        shaderGrid->UploadUniformMatrix4fv("u_Model", camera.GetViewProjectionMatrix()); 
        shaderGrid->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderGrid->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderGrid->UploadUniform1i("u_Texture", gridTextureUnit);
        RenderCommands::DrawIndex(GL_TRIANGLES, VAO_Grid);

        // handle cube rotation, translations with the model matrix
        if (hasMoved || hasRotated)
        {
            model = glm::mat4(1.0f);

            // helper
            float sideLength = 2.0f / static_cast<float>(numberOfSquare); 
            float bottomRight = numberOfSquare * sideLength -1; 

            // Update translation to position the cube relative to the bottom right corner
            //TODO: if everything would have been set up correct "- sideLength * numberOfSquare" (basically an additional offset) 
            // should not have been neccessary. ("+ sideLength / 2" is neccessary because the ccube spawns in the origin 
            // and therefore on an edge of the grid)
            float translationX = bottomRight - currentXSelected * sideLength - sideLength / 2; 
            float translationY = bottomRight + currentYSelected * sideLength + sideLength / 2 - sideLength * numberOfSquare;
            model = glm::translate(model, glm::vec3(translationX, translationY, sideLength / 2 + 0.01f));

            // Apply rotation to the cube
            model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));

            // scaling
            float scaleValue = 2.0f / static_cast<float>(numberOfSquare);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue, scaleValue, scaleValue));
            model *= scaleMatrix;

            hasMoved = false;
            hasRotated = false;
        }


        // bind Cube Buffers, upload cube uniforms, draw cube
        VAO_Cube->Bind(); 
        shaderCube->Bind();
        shaderCube->UploadUniformMatrix4fv("u_Model", model); 
        shaderCube->UploadUniformMatrix4fv("u_View", camera.GetViewMatrix());
        shaderCube->UploadUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());
        shaderCube->UploadUniformFloat4("u_CubeColor", cubeColor);
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
