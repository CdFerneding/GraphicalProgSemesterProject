#ifndef PROG2002_GLFWAPPLICATION_H
#define PROG2002_GLFWAPPLICATION_H
#define GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>
#include <iostream>

class GLFWApplication
{
public:
    explicit GLFWApplication(const std::string &name = "GLFWApplication", const std::string &version = "0.0.1", unsigned int width = 800, unsigned int height = 600);
    ~GLFWApplication();

    // Initialization
    virtual unsigned Init() // Virtual function with default behavior.
    {
        {
            // Initialize GLFW (Graphics Lbrary Framework)
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
            return EXIT_SUCCESS;
        }
    }

    // Run function
    virtual unsigned Run() const = 0; // Pure virtual function that must be redefined.

    virtual unsigned stop()
    {
        glfwTerminate();
        glfwDestroyWindow(window);

        return EXIT_SUCCESS;
    }

protected:
    //...other functions...
    const std::string &name;
    const std::string &version;
    GLFWwindow* window;
    GLuint program;
    unsigned width;
    unsigned height;
};



#endif //PROG2002_GLFWAPPLICATION_H
