#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) 
{

    // Convert shader source code from std::string to raw char pointer.
    auto vertexRaw = vertexSrc.c_str();
    auto fragmentRaw = fragmentSrc.c_str();
    // Compile vertex shader, fragment shader
    VertexShader = CompileShader(GL_VERTEX_SHADER, vertexRaw);
    FragmentShader =  CompileShader(GL_FRAGMENT_SHADER, fragmentRaw);

    // Create shader program and attach compiled shaders
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
    GLint success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

}


Shader::~Shader()
{
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    glDeleteProgram(ShaderProgram);
}


void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}


void Shader::Unbind() const
{
    glUseProgram(0);
}


void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector)
{
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}


GLuint Shader::CompileShader(GLenum shaderType, const char * shaderSrc)
{
    auto shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc , nullptr);
    glCompileShader(shader);
    return shader;
}