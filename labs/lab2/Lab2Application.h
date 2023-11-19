#ifndef PROG2002_LAB2APPLICATION_H
#define PROG2002_LAB2APPLICATION_H

#include <string>
#include <vector>
#include "GLFWApplication.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Lab2Application : public GLFWApplication {
    private:
        unsigned int currentXSelected;
        unsigned int currentYSelected;
        const unsigned int numberOfSquare = 8;
        static Lab2Application * current_application;
        bool hasMoved;

        std::vector<float> createSelectionSquare() const;
    public:
        explicit Lab2Application(const std::string &name = "Lab2", const std::string &version = "0.0.1",
                                 unsigned int width = 800, unsigned int height = 800);
        ~Lab2Application();
        unsigned Run();
        unsigned stop();
        void move(Direction direction);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static Lab2Application * getLab2Application();
};



#endif //PROG2002_LAB2APPLICATION_H
