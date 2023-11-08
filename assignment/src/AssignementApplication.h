#ifndef PROG2002_LAB3APPLICATION_H
#define PROG2002_LAB3APPLICATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include "GLFWApplication.h"
#include "VertextArray.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class AssignementApplication : public GLFWApplication {
private:
    unsigned int currentXSelected;
    unsigned int currentYSelected;

    float rotationAngleX;
    float currentRotationAngleX;
    float rotationAngleY;
    float currentRotationAngleY;

    const unsigned int numberOfSquare = 8;
    static AssignementApplication* current_application;
    bool hasMoved = false;
    bool hasCameraChanged = false;
    std::unordered_map<unsigned int, std::shared_ptr<VertexArray>> cubes= {};
    std::unordered_map<unsigned int, bool> cubesColor= {};

    unsigned int currentCubeSelected = -1; //idx of the cube selected
    std::shared_ptr<VertexArray> currentSelectedVertexArray;
    bool hasCubeSelected = false;
    void updateSelectedCube();
    bool colorSelectedCube;
    std::array<int, 2> previousPosition = {-1, -1};
    std::vector<float> createSelectionSquare() const;
    std::vector<float> createSelectionCube(float r, float g, float b, unsigned int x, unsigned int y) const;
public:
    explicit AssignementApplication(const std::string& name = "Lab3", const std::string& version = "0.0.1",
        unsigned int width = 800, unsigned int height = 600);
    ~AssignementApplication();
    unsigned Run();
    unsigned stop();
    void move(Direction direction);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static AssignementApplication* getAssignementApplication();
    void zoom(float zoomValue);
    void rotate(float degree);
    void select();
    void exit();
};



#endif //PROG2002_LAB3APPLICATION_H
