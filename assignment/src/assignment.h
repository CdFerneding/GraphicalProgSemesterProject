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
#include <glm/glm.hpp>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class AssignementApplication : public GLFWApplication {
private:
    // Define a struct to store information about each unit
    struct UnitInfo {
        glm::vec3 teamColor;
        glm::vec3 currentColor;
        glm::vec2 currentPosition;
        bool selected;
    };
    std::vector<UnitInfo> unitInfoVector;
    glm::vec3 colorUnitSelected = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 colorUnitHover = glm::vec3(0.0f, 1.0f, 0.0f);

    unsigned int currentXSelected; // Current x position of the selector
    unsigned int currentYSelected; // Current y position of the selector

    PerspectiveCamera camera; //The perspective camera used

    bool hasMoved; // True when the player is moving the selection square
    bool hasCameraChanged; // True when the player is changing the camera rotation / zoom
    bool isUnitSelected;
    float toggleTexture; // toggle functionality to activate/deactivate textures and blending

    std::array<std::shared_ptr<VertexArray>, 8*8> cubes= {nullptr}; // A list of vertex array containing the cubes. To get a specific one, you can use the vertexArrayIdPerCoordinate

    int vertexArrayIdPerCoordinate[8][8] = {{-1},}; // An array containing the vertexArrayId used by a cube for a specific coordinate

    std::vector<std::shared_ptr<VertexArray>> vertexArrays; // A list of vertex array. To get a specific one, you can use the vertexArrayIdPerCoordinate

    std::vector<bool> colorVertexArrays; // An array containing the color of each cube according to his vertexArrayId

    std::shared_ptr<VertexArray> currentSelectedVertexArray; // The vertex array of the selected cube

    std::array<int, 2> moveUnitFrom; // Contain the previous position of the selected cube, (-1,-1) otherwise
    std::array<unsigned, 2> previousPositionSelector; // The previous position of the selection square

    std::shared_ptr<VertexBuffer> VBO_SelectionSquare; // The vertex buffer of the selection square
    std::vector<float> selectionSquare; // The content of the vertexBufferSelectionSquare


    const unsigned int numberOfSquare = 8; // The number of square on the grid

    static AssignementApplication* current_application; // The current_application used for the communication with the key_callback

    /**
     * Create the green selection square on the board according to the current X and Y position
     * @param opacity The opacity of the square (0 if there is a cube, 1 if there isn't a cube)
     * @return The content for the vertex buffer of the selection square
     */
    [[nodiscard]] std::vector<float> createSquare(float opacity) const;

    /** 
    creates a unit at the origin
    (neccessary to apply the cubemap)
    */
    std::vector<float> createUnit() const;
    int selectUnit(); 
    int moveUnit();
    void setupUnits();

    /**
     * Function called when the player press Enter and want to move a cube (selecting the cube or setting the destination of the cube)
     */
    void moveCubeRequest();
public:
    explicit AssignementApplication(const std::string& name = "assignment", const std::string& version = "0.0.1",
        unsigned int width = 800, unsigned int height = 600);

    ~AssignementApplication();

    unsigned Run() override;
    unsigned stop() override;

    /**
     * Move the selection square in a specific direction
     * @param direction The direction to move the selection square
     */
    void move(Direction direction);

    /**
     * Function called when the player press any key on the keyboard
     */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void setTextureState();
    /**
     * Function called when the key callback want to use a function of the current_application
     * @return current_application
     */
    static AssignementApplication* getAssignementApplication();

    /**
     * Function called by the key_callback when the player try to zoom in or out.
     * This function change the fov of the camera
     * @param zoomValue The value to add to the current fov
     */
    void zoom(float zoomValue);

    /**
     * Function called by the key_callback when the player try to rotate the camera around the lookAt point
     * It change the position of the camera
     * @param degree The value to add to the current rotation
     */
    void rotate(float degree);

    /**
     * Function called by the key_callback when the player try to select a cube
     */
    void select();

    /**
     * Function called by the key_callback when the player want to exit the application
     */
    void exit();
};



#endif //PROG2002_LAB3APPLICATION_H
