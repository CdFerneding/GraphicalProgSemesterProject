# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

# Declare a new project named 'tuto_lab1_glfw'. This sets various variables
# related to the project and provides a context for the following CMake
# commands.
project(lab2)
# Add an executable target named 'tuto_lab1_glfw' that is built from the source
# file 'main.cpp'. This means that CMake will generate build rules to compile
# 'main.cpp' and link it into an executable program named 'tuto_lab1_glfw'.
add_executable(lab2 main.cpp Lab2Application.h Lab2Application.cpp shader.h)

# Specify libraries to link with the 'tuto_lab1_glfw' target.
# This tells CMake that when it's linking 'tuto_lab1_glfw', it should also
# link the following libraries:
# - glfw: This is a windowing library commonly used with OpenGL.
# - glad: A library to load OpenGL extensions.
# - OpenGL::GL: This is an imported target for the main OpenGL library
#               provided by the find_package(OpenGL) command.
target_link_libraries(${PROJECT_NAME} PRIVATE Framework::GLFWApplication glfw glad OpenGL::GL)