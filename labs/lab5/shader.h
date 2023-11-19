#include <string>
#ifndef PROG2002_SHADERLAB5_H
#define PROG2002_SHADERLAB5_H

// Vertex and fragment shader source code
const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    out vec4 fragColor;
    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;
    uniform float u_ambientStrength;
    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        fragColor = u_ambientStrength * color;
        fragColor.a = color.a; // The light doesn't change how transparent a vertex is
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


#endif //PROG2002_SHADERLAB5_H
