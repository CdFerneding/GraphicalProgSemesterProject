#include <string>
#ifndef PROG2002_SHADERLAB2_H
#define PROG2002_SHADERLAB2_H

// Vertex and fragment shader source code
const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    out vec4 fragColor;
    void main()
    {
        gl_Position = vec4(position, 1.0);
        fragColor = color;
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core
    in vec4 fragColor;
    out vec4 color;
    void main()
    {
        color = fragColor;
    }
)";


#endif //PROG2002_SHADERLAB2_H
