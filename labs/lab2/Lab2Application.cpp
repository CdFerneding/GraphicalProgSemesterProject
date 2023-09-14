#include "Lab2Application.h"

int main(int argc, char *argv[])
{
    Lab2Application application("Lab1", "1.0");

    application.Init();
    return application.Run();
}

Lab2Application::Lab2Application(const std::string &name, const std::string &version) : GLFWApplication(name, version) {

}

Lab2Application::~Lab2Application() {

}

unsigned Lab2Application::Init() {
    return GLFWApplication::Init();
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
