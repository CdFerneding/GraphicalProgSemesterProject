#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) 
{
    // Convert shader source code from std::string to raw char pointer.
    auto vertexRaw = vertexSrc.c_str();
    auto fragmentRaw = fragmentSrc.c_str();

    // Compile vertex shader, fragment shader
    CompileShader(GL_VERTEX_SHADER, vertexRaw);
    CompileShader(GL_FRAGMENT_SHADER, fragmentRaw);

    // Create shader program and attach compiled shaders
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

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


void Shader::CompileShader(GLenum shaderType, const std::string& shaderSrc)
{
    auto shader = glCreateShader(shaderType);
    auto src = shaderSrc.c_str();
    glShaderSource(shader, 1, &src , nullptr);
    glCompileShader(shader);
}