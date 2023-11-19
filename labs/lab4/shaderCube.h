#include <string>
#ifndef PROG2002_SHADERCUBE_H
#define PROG2002_SHADERCUBE_H

// Vertex and fragment shader source code
const std::string VS_Cube = R"(
    #version 430 core
    layout(location = 0) in vec3 position;

    out vec3 TexCoords;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        TexCoords = position;
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    }
)";

const std::string FS_Cube = R"(
    #version 430 core

    in vec3 TexCoords;
    
    uniform samplerCube CubeMap;
    uniform vec4 u_CubeColor;

    out vec4 color;
    
    void main()
    {
        // Sample the texture using the texture coordinates
        vec4 texColor = texture(CubeMap, TexCoords);
        
        color = mix(vec4(texColor.rgb, 0.5), u_CubeColor, 0.3);
    }
)";


#endif //PROG2002_SHADERCUBE_H
