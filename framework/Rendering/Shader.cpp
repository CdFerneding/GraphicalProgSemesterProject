#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) 
{

    // Convert shader source code from std::string to raw char pointer.
    auto vss = vertexShaderSrc.c_str();
    auto fss = fragmentShaderSrc.c_str();
    // Compile vertex shader, fragment shader
    VertexShader = CompileShader(GL_VERTEX_SHADER, vss);
    FragmentShader =  CompileShader(GL_FRAGMENT_SHADER, fss);

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
    glDeleteShader(VertexShader); 
    glDeleteShader(FragmentShader); 
    glUseProgram(ShaderProgram);
}


Shader::~Shader()
{
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

void Shader::UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) {
    GLuint location = glGetUniformLocation(ShaderProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


GLuint Shader::CompileShader(GLenum shaderType, const char * shaderSrc)
{
    auto shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc , nullptr);
    glCompileShader(shader);
    return shader;
}