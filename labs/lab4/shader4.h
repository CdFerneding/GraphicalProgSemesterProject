#include <string>
#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

// Vertex and fragment shader source code
const std::string vertexShaderSrc = R"(
    #version 430 core
    layout(location = 0) in vec3 position;
    //layout(location = 1) in vec4 color;
    layout(location = 1) in vec2 texCoords;

    //out vec4 fragColor;
    out vec2 fragTexCoords;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;

    void main()
    {
        gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
        //fragColor = color;
        fragTexCoords = texCoords;
    }
)";

const std::string fragmentShaderSrc = R"(
    #version 430 core

    //in vec4 fragColor;
    in vec2 fragTexCoords;
    
    layout(binding=0) uniform sampler2D uTexture;

    out vec4 color;
    
    void main()
    {
        // Sample the texture using the texture coordinates
        vec4 texColor = texture(uTexture, fragTexCoords);
        color = texColor;
    }
)";


#endif //PROG2002_SHADER_H
