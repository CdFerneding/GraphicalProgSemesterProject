#include "Lab3Application.h"
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

Lab3Application* Lab3Application::current_application = nullptr;

Lab3Application::Lab3Application(const std::string& name, const std::string& version,
    unsigned int width, unsigned int height) : GLFWApplication(name, version, width, height) {
    currentXSelected = 0;
    currentYSelected = 0;
    rotationAngleX = 0;
    rotationAngleY = 0;
    currentRotationAngleX = 0;
    currentRotationAngleY = 0;
}

Lab3Application::~Lab3Application() {

}
void Lab3Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_UP:
            getLab3Application()->move(UP);
            break;
        case GLFW_KEY_DOWN:
            getLab3Application()->move(DOWN);
            break;
        case GLFW_KEY_LEFT:
            getLab3Application()->move(LEFT);
            break;
        case GLFW_KEY_RIGHT:
            getLab3Application()->move(RIGHT);
            break;

        // There is two key for up and left because we are using a QWERTY and an AZERTY keyboard
        case GLFW_KEY_Z:
        case GLFW_KEY_W:
		    getLab3Application()->rotateCube(UP);
		    break;
        case GLFW_KEY_A:
        case GLFW_KEY_Q:
            getLab3Application()->rotateCube(LEFT);
            break;
        case GLFW_KEY_S:
            getLab3Application()->rotateCube(DOWN);
            break;
        case GLFW_KEY_D:
            getLab3Application()->rotateCube(RIGHT);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            break;
        }
    }
}
void Lab3Application::move(Direction direction) {
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

void Lab3Application::rotateCube(Direction direction) {
    hasRotated = true;
    switch (direction) {
        case UP:
			rotationAngleX = ( (int) rotationAngleX - 5) % 360;
			break;
        case DOWN:
            rotationAngleX = ((int) rotationAngleX + 5) % 360;
            break;
        case LEFT:
			rotationAngleY = ((int) rotationAngleY - 5) % 360;
			break;
        case RIGHT:
			rotationAngleY = ((int) rotationAngleY + 5) % 360;
			break;
		default:
			break;
    }
}

std::vector<float> Lab3Application::createSelectionSquare() const {

    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.0, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.0, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.0, 1, 0, 0, 1
    };

    //print the selection square
    /*for (int i = 0; i < selectionSquare.size(); i+=7) {
        std::cout << selectionSquare[i] << ", " << selectionSquare[i+1] << ", " << selectionSquare[i+2] << std::endl;
    }*/

    return selectionSquare;
}

std::vector<float> Lab3Application::createSelectionCube() const {
    std::vector<float> selectionSquare = {
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.1, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 0.1, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  0.1, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 0.1, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 2.0f / (float)numberOfSquare, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)), 2.0f / (float)numberOfSquare, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected + 1)),  2.0f / (float)numberOfSquare, 1, 0, 0, 1,
            1 - (2.0f / (float)numberOfSquare) * (float(currentXSelected + 1)), -1 + (2.0f / (float)numberOfSquare) * (float(currentYSelected)), 2.0f / (float)numberOfSquare, 1, 0, 0, 1
    };
    return selectionSquare;
}

unsigned Lab3Application::Run() {
    current_application = this;
    hasRotated = true;
    hasMoved = true;

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

    auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
    auto gridBufferLayout = std::make_shared<BufferLayout>(BufferLayout({
        {ShaderDataType::Float3, "position", false}
        ,{ShaderDataType::Float4, "color", false} // When we use the color in the vertexBuffer
        }));

    auto vertexBuffer = std::make_shared<VertexBuffer>(triangle.data(), sizeof(float) * triangle.size());

    auto cube = createSelectionCube();

    auto vertexArrayCube = std::make_shared<VertexArray>();

    auto indicesCube = GeometricTools::CubeTopology;

    auto indexBufferCube = std::make_shared<IndexBuffer>(indicesCube.data(), indicesCube.size());

    auto vertexBufferCube = std::make_shared<VertexBuffer>(cube.data(), sizeof(float) * cube.size());

    vertexBufferCube->SetLayout(*gridBufferLayout);
    vertexArrayCube->AddVertexBuffer(vertexBufferCube);
    vertexArrayCube->SetIndexBuffer(indexBufferCube);

    vertexArrayCube->Bind();

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

    glfwSetKeyCallback(window, Lab3Application::key_callback);

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
            selectionSquare = createSelectionCube();
            //Update vertexbufferCube with the content of createCube
            vertexBufferCube->BufferSubData(0,
                				sizeof(float) * 7 * 8, selectionSquare.data());


            hasMoved = false;
            hasRotated = true;
            currentRotationAngleX = 0;
            currentRotationAngleY = 0;
        }

        if (hasRotated) {
            // Get the gravity center of the cube
            float gravityPoint[3] = {0.0f, 0.0f, 0.0f};

            for (int i = 0; i < selectionSquare.size(); i+=7) {
                gravityPoint[0] += selectionSquare[i];
                gravityPoint[1] += selectionSquare[i+1];
                gravityPoint[2] += selectionSquare[i+2];
            }

            gravityPoint[0] /= 8;
            gravityPoint[1] /= 8;
            gravityPoint[2] /= 8;

            //To rotate the cube around its center, we need to translate the cube to the origin, rotate it and translate it back to its original position

            selectionSquare = GeometricTools::translateCube(selectionSquare, -gravityPoint[0], -gravityPoint[1], -gravityPoint[2]);

            //Rotate the cube

            selectionSquare = GeometricTools::rotateCube(selectionSquare, rotationAngleX-currentRotationAngleX, rotationAngleY-currentRotationAngleY, 0);

            //Translate the cube back to its original position

            selectionSquare = GeometricTools::translateCube(selectionSquare, gravityPoint[0], gravityPoint[1], gravityPoint[2]);

            //Update vertexbufferCube with the content of new_cube
			vertexBufferCube->BufferSubData(0,
                								sizeof(float) * 7 * 8, selectionSquare.data());

            currentRotationAngleX = rotationAngleX;
            currentRotationAngleY = rotationAngleY;

			hasRotated = false;
        }

        RenderCommands::DrawIndex(
            GL_TRIANGLES,
            vertexArrayCube
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

unsigned Lab3Application::stop() {
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

Lab3Application* Lab3Application::getLab3Application() {
    return Lab3Application::current_application;
}
