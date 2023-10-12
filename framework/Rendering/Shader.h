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
	void UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix);

private:
	GLuint VertexShader;
	GLuint FragmentShader;
	GLuint ShaderProgram;

	GLuint CompileShader(GLenum shaderType, const char * shaderSrc);
};

