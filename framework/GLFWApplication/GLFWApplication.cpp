#include "GLFWApplication.h"
#include "glad/glad.h"


GLFWApplication::~GLFWApplication() = default;

GLFWApplication::GLFWApplication(const std::string &name, const std::string &version, unsigned int width, unsigned int height): name(name), version(version), width(width), height(height) {
    window = nullptr;
    program = 0;
}

unsigned int GLFWApplication::Init() {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // GLFW initialization code (SECTION 2)

    // OpenGL initialization code (SECTION 3)
    if (glfwInit() == GLFW_FALSE) return EXIT_FAILURE;
    window = glfwCreateWindow(width, height, (name+" - "+version).c_str(), NULL, NULL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwMakeContextCurrent(window);
    return EXIT_SUCCESS;
}

unsigned int GLFWApplication::stop() {
    glfwTerminate();
    glfwDestroyWindow(window);

    return EXIT_SUCCESS;
}
