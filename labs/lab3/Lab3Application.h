#ifndef PROG2002_LAB3APPLICATION_H
#define PROG2002_LAB3APPLICATION_H

#include <string>
#include <vector>
#include "GLFWApplication.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Lab3Application : public GLFWApplication {
private:
    unsigned int currentXSelected;
    unsigned int currentYSelected;

    float rotationAngleX;
    float currentRotationAngleX;
    float rotationAngleY;
    float currentRotationAngleY;

    const unsigned int numberOfSquare = 8;
    static Lab3Application* current_application;
    bool hasMoved;
    bool hasRotated;

    std::vector<float> createSelectionSquare() const;
    std::vector<float> createSelectionCube() const;
public:
    explicit Lab3Application(const std::string& name = "Lab2", const std::string& version = "0.0.1",
        unsigned int width = 800, unsigned int height = 800);
    ~Lab3Application();
    unsigned Run();
    unsigned stop();
    void move(Direction direction);
    void rotateCube(Direction direction);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static Lab3Application* getLab3Application();
};



#endif //PROG2002_LAB3APPLICATION_H
