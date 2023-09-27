#include "GLFWApplication.h"
#include "glad/glad.h"
#include <iostream>


GLFWApplication::~GLFWApplication() = default;

GLFWApplication::GLFWApplication(const std::string& name, const std::string& version, unsigned int width, unsigned int height) : name(name), version(version), width(width), height(height) {
    window = nullptr;
    program = 0;
}

unsigned int GLFWApplication::Init() {
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

unsigned int GLFWApplication::stop() {
    glfwTerminate();
    glfwDestroyWindow(window);

    return EXIT_SUCCESS;
}



