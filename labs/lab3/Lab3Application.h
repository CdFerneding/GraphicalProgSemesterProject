#ifndef PROG2002_LAB3APPLICATION_H
#define PROG2002_LAB3APPLICATION_H

#include <string>
#include "GLFWApplication.h"

class Lab3Application : public GLFWApplication{
    public:
        explicit Lab3Application(const std::string &name = "Lab2", const std::string &version = "0.0.1",
                                 unsigned int width = 800, unsigned int height = 600);
        ~Lab3Application();
        unsigned Run();
};


//
// Function to implement programmed shaders: virtual function with default behaviour
//
GLuint CompileShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);


#endif //PROG2002_LAB3APPLICATION_H
