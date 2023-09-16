#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int main(int argc, char** argv)
{
    // Initialize GLFW
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

    // Vertex data for a triangle
    float triangle[3 * 2] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f,  0.5f
    };

    // Create a vertex array object (VAO)
    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    // Create a vertex buffer object (VBO)
    GLuint vertexBufferId;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // Define the vertex attribute layout
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);

    // Vertex and fragment shader source code
    const std::string vertexShaderSrc = R"(
        #version 430 core
        layout(location = 0) in vec2 position;
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
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up GLFW and exit
    glfwTerminate();
    return EXIT_SUCCESS;
}
