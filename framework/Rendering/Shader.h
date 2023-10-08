#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;
	void UploadUniformFloat2(const std::string& name, const glm::vec2& vector);

private:
	GLuint VertexShader;
	GLuint FragmentShader;

public:
	GLuint ShaderProgram;

	void CompileShader(GLenum shaderType, const std::string& shaderSrc);
};

