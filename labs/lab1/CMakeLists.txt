# Set the minimum required version of CMake that the project can use.
# This ensures that the version of CMake used to build the project has
# all the features that are required.
cmake_minimum_required(VERSION 3.15)

# Declare a new project named 'tuto_lab1_glfw'. This sets various variables
# related to the project and provides a context for the following CMake
# commands.
project(lab1)

# Search for the OpenGL package on the system. The REQUIRED argument
# ensures that if OpenGL is not found, CMake will produce an error and
# stop processing. If found, this command sets up various variables
# and imported targets related to OpenGL that can be used later.
find_package(OpenGL REQUIRED)

add_executable(lab1 main.cpp)   # Generate an executable named HelloWorld from main.cpp [7]

# issue 2: creating a simple OpenGL application
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
