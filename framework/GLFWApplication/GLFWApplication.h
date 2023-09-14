#ifndef PROG2002_GLFWAPPLICATION_H
#define PROG2002_GLFWAPPLICATION_H
#define GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <string>

class GLFWApplication
{
public:
    GLFWApplication(const std::string &name, const std::string &version);
    ~GLFWApplication();

    // Initialization
    virtual unsigned Init(); // Virtual function with default behavior.

    // Run function
    virtual unsigned Run() const = 0; // Pure virtual function that must be redefined.

    virtual unsigned stop();

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
