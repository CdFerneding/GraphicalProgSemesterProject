// #include area (SECTION 1)

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../external/linmath.h"
#include <iostream>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
        {
                { -0.6f, -0.4f, 1.f, 0.f, 0.f },
                {  0.6f, -0.4f, 0.f, 1.f, 0.f },
                {   0.f,  0.6f, 0.f, 0.f, 1.f }
        };

static const char* vertex_shader_text =
        "#version 110\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragment_shader_text =
        "#version 110\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv)
{
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;

    GLint mvp_location, vpos_location, vcol_location;

    if (!glfwInit())
    {
        // Initialization failed
        std::cerr << "GLFW initialization failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(error_callback);
    // OpenGL initialization code (SECTION 3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);    if (!window)
    {
        std::cerr << "GLFW window creation failed" << std::endl;
        return 1;
        // Window or OpenGL context creation failed
    }

    // GLFW initialization code (SECTION 2)

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    // Glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    glfwSwapInterval(1);

    // OpenGL data transfer code (SECTION 4)

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "95, OpenGL Error: %d\n", err);

    }
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    err = glGetError();
    if (err != GL_NO_ERROR) {

        fprintf(stderr, "105, OpenGL Error: %d\n", err);

    }
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "114, OpenGL Error: %d\n", err);

    }
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "124, OpenGL Error: %d\n", err);

    }
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "133, OpenGL Error: %d\n", err);

    }
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "141, OpenGL Error: %d\n", err);

    }
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));


    std::cout << "Current Version of OpenGL" << glGetString(GL_VERSION) << std::endl;

    err = glGetError();
    if (err != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL Error: %d\n", err);

    }

    // Application loop code (SECTION 5)
    while (!glfwWindowShouldClose(window))
    {

        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Termination code (SECTION 6)
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

