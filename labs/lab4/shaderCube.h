#include <string>
#ifndef PROG2002_SHADERCUBE_H
#define PROG2002_SHADERCUBE_H

// Vertex and fragment shader source code
const std::string VS_Cube = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;

    out vec4 fragColor;
    out vec3 TexCoords;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        TexCoords = position;
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        fragColor = color;
    }
)";

const std::string FS_Cube = R"(
    #version 430 core

    in vec4 fragColor;
    in vec3 TexCoords;
    
    uniform samplerCube CubeMap;

    out vec4 color;
    
    void main()
    {
        // Sample the texture using the texture coordinates
        vec4 texColor = texture(CubeMap, TexCoords);
        
        color = texColor;
        //color = fragColor;

        // Mix the color from the vertex attribute with the texture color
        // Adjust the alpha value manually
        //vec4 mixedColor = mix(fragColor, vec4(texColor.rgb, 1.0), 0.6);

        // Output the final color
        //color = mixedColor;
    }
)";


#endif //PROG2002_SHADERCUBE_H
