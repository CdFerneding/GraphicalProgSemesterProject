#include <iostream>

//int main(int argc, char** argv)
//{
//	std::cout << "Hello World!" << std::endl;
//	std::cin.get();
//	return EXIT_SUCCESS;
//}


// #include area (SECTION 1)
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>

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
