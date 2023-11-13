#include <string>
#ifndef PROG2002_SHADERLAB4_H
#define PROG2002_SHADERLAB4_H

// Vertex and fragment shader source code
const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;
    layout(location = 2) in vec2 texCoords;

    out vec3 fragTexCoords;
    out vec4 fragColor;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        fragTexCoords = vec3(texCoords, 1.0);
        fragColor = color;
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    in vec3 fragTexCoords;
    in vec4 fragColor;
    
    uniform samplerCube CubeMap;

    out vec4 color;
    
    void main()
    {
        // Sample the texture using the texture coordinates
        vec4 texColor = texture(CubeMap, fragTexCoords);
        
        //color = texColor;
        //color = fragColor;

        // Mix the color from the vertex attribute with the texture color
        // Adjust the alpha value manually
        vec4 mixedColor = mix(fragColor, vec4(texColor.rgb, 1.0), 0.6);

        // Output the final color
        color = mixedColor;
        
    }
)";


#endif //PROG2002_SHADERLAB4_H
