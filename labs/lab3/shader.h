#include <string>
#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

// Vertex and fragment shader source code
const std::string vertexShaderSrc = R"(
        #version 430 core
        layout(location = 0) in vec4 a_Position;
        uniform mat4 u_Model;
        uniform mat4 u_View;
        uniform mat4 u_Projection;
        void main(){
            gl_Position = u_Projection* u_View * u_Model * position;
        }
    )";

const std::string fragmentShaderSrc = R"(
        #version 430 core
        out vec4 color;
        void main()
        {
            color = vec4(1.0, 0.5, 0.0, 1.0);
        }
    )";

#endif //PROG2002_SHADER_H
