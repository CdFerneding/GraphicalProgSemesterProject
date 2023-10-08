#ifndef PROG2002_LAB2APPLICATION_H
#define PROG2002_LAB2APPLICATION_H

#include <string>
#include "GLFWApplication.h"

class Lab2Application : public GLFWApplication{
    public:
        explicit Lab2Application(const std::string &name = "Lab2", const std::string &version = "0.0.1", 
            unsigned int width = 800, unsigned int height = 600);
        ~Lab2Application();
        unsigned Run();
        unsigned stop();
};


//
// Function to implement programmed shaders: virtual function with default behaviour
//
GLuint CompileShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);


#endif //PROG2002_LAB2APPLICATION_H
