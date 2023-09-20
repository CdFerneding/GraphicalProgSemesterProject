#include "Lab2Application.h"
#include "shader.h"

Lab2Application::Lab2Application(const std::string &name, const std::string &version, unsigned int width, unsigned int height): GLFWApplication(name, version, width, height) {

}

Lab2Application::~Lab2Application() {

}

unsigned Lab2Application::Init() {
    auto result = GLFWApplication::Init();


    // Load shaders and setup buffer


    return result;
}

unsigned Lab2Application::Run() const {

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Keep the application running




    }
    return 0;
}

unsigned Lab2Application::stop() {
    GLFWApplication::stop();
    return 0;
}
