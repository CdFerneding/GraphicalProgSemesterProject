// #include <iostream>

//int main(int argc, char** argv)
//{
//	std::cout << "Hello World!" << std::endl;
//	std::cin.get();
//	return EXIT_SUCCESS;
//}


// #include area (SECTION 1)
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>


int main(int argc, char** argv)
{
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// GLFW initialization code (SECTION 2)

	// OpenGL initialization code (SECTION 3)
	if (glfwInit() == GLFW_FALSE) return EXIT_FAILURE;
	auto window = glfwCreateWindow(800, 600, "I'm a window", NULL, NULL);
	// glfwWindowHint(int hint, int value);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwMakeContextCurrent(window);

	// OpenGL data transfer code (SECTION 4)
	float triangle[3 * 2] = { // 3 vertices x 2 coordinate components
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f,  0.5f
	};

	// Create a vertex array (transfer the triangle from CPU to GPU). VAO (Vertex Array Object):
	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	// Create a vertex buffer. VBO (Vertex Buffer Object):
	GLuint vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

	// Populate the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	// Set the layout of the bound buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
	glEnableVertexAttribArray(0);

	// Shaders:

	const std::string vertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec2 position;

void main()
{
  gl_Position = vec4(position, 0.0, 1.0); // Homogeneous coordinates 3D+1
}
)";

	const std::string fragmentShaderSrc = R"(
#version 430 core

out vec4 color;
void main()
{
  color = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

	// Compile the vertex shader
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vss = vertexShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vss, nullptr);
	glCompileShader(vertexShader);

	// Compile the fragment shader
	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fss = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShader, 1, &fss, nullptr);
	glCompileShader(fragmentShader);

	// Create a shader program
	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	// Shader objects can be deleted once they 
// have been linked in a shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	// drawing the triangle
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glfwSwapBuffers(window);


	// Application loop code (SECTION 5)
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Keep the application running
	}

	// Termination code (SECTION 6)
	glfwTerminate();
	glfwDestroyWindow(window);

	return EXIT_SUCCESS;
}
