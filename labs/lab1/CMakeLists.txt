add_executable(HelloWorld main.cpp)   # Generate an executable named HelloWorld from main.cpp [7]

# issue 2: creating a simple OpenGL application
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
