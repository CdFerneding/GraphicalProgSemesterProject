#ifndef PROG2002_LAB3APPLICATION_H
#define PROG2002_LAB3APPLICATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include "GLFWApplication.h"
#include "VertextArray.h"
#include "Shader.h"
#include "PerspectiveCamera.h"

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

    Shader *shader;
    PerspectiveCamera camera;

    const unsigned int numberOfSquare = 8;
    static AssignementApplication* current_application;
    bool hasMoved = false;
    bool hasCameraChanged = false;
    std::array<std::shared_ptr<VertexArray>, 8*8> cubes= {nullptr};

    int currentCubeSelected = -1; //idx of the cube selected
    std::shared_ptr<VertexArray> currentSelectedVertexArray;
    int vertexArrayIdPerCoordinate[8][8] = {{-1},};
    std::vector<std::shared_ptr<VertexArray>> vertexArrays;
    std::vector<bool> colorVertexArrays;
    bool hasCubeSelected = false;

    bool cubeJustPlaced = false;

    void updateSelectedCube();

    std::array<int, 2> previousPosition = {-1, -1};
    std::array<unsigned, 2> previousPositionSelector = {0, 0};
    std::vector<float> createSelectionSquare(float opacity) const;
    std::vector<float> createSelectionCube(float r, float g, float b, unsigned int x, unsigned int y) const;
    std::shared_ptr<VertexBuffer> vertexBufferSelectionSquare;
    std::vector<float> selectionSquare;
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

    bool hasJustSelected;
};



#endif //PROG2002_LAB3APPLICATION_H
