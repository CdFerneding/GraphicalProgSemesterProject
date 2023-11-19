#ifndef PROG2002_LAB5APPLICATION_H
#define PROG2002_LAB5APPLICATION_H

#include <string>
#include <vector>
#include "GLFWApplication.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Lab5Application : public GLFWApplication {
private:
    unsigned int currentXSelected;
    unsigned int currentYSelected;

    float rotationAngleX;
    float currentRotationAngleX;
    float rotationAngleY;
    float currentRotationAngleY;

    const unsigned int numberOfSquare = 8;
    static Lab5Application* current_application;
    bool hasMoved;
    bool hasRotated;

    std::vector<float> createSelectionSquare() const;
    std::vector<float> createSelectionCube() const;
public:
    explicit Lab5Application(const std::string& name = "Lab5", const std::string& version = "0.0.1",
        unsigned int width = 800, unsigned int height = 800);
    ~Lab5Application();
    unsigned Run();
    unsigned stop();
    void move(Direction direction);
    void rotateCube(Direction direction);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Lab5Application* getLab5Application();
};



#endif //PROG2002_LAB5APPLICATION_H
