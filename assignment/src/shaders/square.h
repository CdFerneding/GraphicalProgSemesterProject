#include <string>
#ifndef PROG2002_SHADERCUBE_H
#define PROG2002_SHADERCUBE_H

// Vertex and fragment shader source code
const std::string VS_Square = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;

    out vec4 fragColor

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        fragColor = color;
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    }
)";

const std::string FS_Square = R"(
    #version 430 core

    in vec3 fragColor;

    out vec4 color;
    
    void main()
    {
        color = fragColor;
    }
)";


#endif //PROG2002_SHADERCUBE_H
