#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "./../GeometricTools/GeometricTools.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int draw_triangle();

int main(int argc, char** argv)
{
    draw_triangle();
}

int draw_triangle()
{

    // Initialize GLFW (Graphics Library Framework)
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // 3 vertexes and 6 vertices
    auto triangle = GeometricTools::UnitTriangle2D; 

    /* "vertex": object that a blob ob byte (which is not only positions) 
    *  it consists of things like position/vertices, colorinfo, ...
    */

    // 
    // vertexBuffer
    //
    GLuint vertexArrayId;
    // generating a buffer
    glGenVertexArrays(  1,                  // specifying the amount of (vertex) buffers you want to create 
                        &vertexArrayId);    // id of the generated buffer / memory address of an unsigned int
   
    glBindVertexArray(vertexArrayId);       // selecting the (vertex) buffer

    //
    // vertex attribute pointer
    // 
    GLuint vertexBufferId; 
    glGenBuffers(1, &vertexBufferId); 
    glBindBuffer(GL_ARRAY_BUFFER,
        vertexBufferId);
    //populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(float) * triangle.size(),    // size of vertices in Bytes (float is 4 B)
        triangle.data(),                    // pointer to our data
        GL_STATIC_DRAW);                    // STATIC: defined exactly one time, drawn multiple times (others: STREAM, DYNAMIC, READ, COPY)
    // Define the vertex attribute layout of the bound buffer
    glVertexAttribPointer(0,        // index of the attribute
        2,                          // the vertex has two float-sized attributes (x-, and y-coordinates)
        GL_FLOAT,                   // type of data in our buffer
        GL_FALSE,                   // normalize if neccessary
        sizeof(float) * 2,          // stride: amount of bytes between every vertexes (not between attributes!)
        nullptr);                   // inside the space of a vertex: offset to first attribute (here you could write also "0" or "(const void*)0"
    glEnableVertexAttribArray(0);   // opbligatory to actually use the written buffer. Can stand anywhere where the buffer was already defined, since openGL is a state machine

    // Vertex and fragment shader source code
    const std::string vertexShaderSrc = R"(
        #version 430 core
        layout(location = 0) in vec2 position; // location: index of the buffer in which the vertexes are we want to use
        void main()
        {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )";

    const std::string fragmentShaderSrc = R"(
        #version 430 core
        out vec4 color;
        void main()
        {
            color = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )";

    /*
    * shaders are programms running on the GPU for handling the packages/buffers we sent him.
    * pipeline: drawcall --> vertex shader --> fragment shader --> restoration/output
    * 
    * vertex shader:
    * code gets called for each vertex we are trying to render (e.g. for a triangle it gets called 3 times). 
    * To tell openGL where we want a vertex to be (in relation to the screen).
    * 
    * fragment shader:
    * 
    */
    // Compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vss = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vss, nullptr);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fss = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fss, nullptr);
    glCompileShader(fragmentShader);

    // Create a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check shader program linking status
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Use the shader program
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        // use glDrawArray when no indexBuffer is used
        glDrawArrays(GL_TRIANGLES,      // "mode of primitives to render"
            0,                          // starting index of vertexes to draw
            3);                         // number of inices/vertexes to render

        // or use
        /*
        * glDrawElements(GL_TRIANGLES,
        *               3,              // number of indices
        *               float,          // type
        *               indices);       // pointer to indices
        */

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up GLFW and exit
    glfwTerminate();
    return EXIT_SUCCESS;
}

/*
* neccessities to have a triangle / "something" on the screen with openGL:
* create a window, make it current
* create data, put it into a buffer (glGenBuffer()) and specify its layout (glVertexAttribPointer())
* create shaders that can actually draw the vertexes in the GPU / VRAM (video RAM)
* a glDrawArrays() / glDrawElements() call on runtime to render the bufferdata with the specified layout
* (some GPU's also provide you with a default shader. better to implement one yourself)
*/