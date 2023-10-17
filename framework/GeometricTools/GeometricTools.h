#ifndef PROG2002_GEOMETRICTOOLS_H
#define PROG2002_GEOMETRICTOOLS_H

#include <array>
#include <vector>

namespace GeometricTools {
    constexpr std::array<float, 3*2> UnitTriangle2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f}; // [2,3]

    constexpr std::array<float, 3*4> UnitSquare2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f, -0.5f,  0.5f}; // [2,4]

    auto UnitSquare2DDivider(const unsigned int divisions) {
        // Divide each value of UnitSquare2D by the value in divisions
        auto square = UnitSquare2D;
        std::vector<float> newSquare;
        for(float i : square) {
            newSquare.push_back(i/(float) divisions);
        }
        return newSquare;
    }

    constexpr std::array<unsigned int, 3> TopologyTriangle2D = {0, 1, 2}; // [3]

    constexpr std::array<unsigned int, 6> TopologySquare2D = {0, 1, 2, 0, 2, 3}; // [6]

    auto UnitGrid2D(const unsigned int divisions) {
        //Create a std vector of float with size of 3*divisions*divisions
        std::vector<float> vertices;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
            }
        }

        //convert grid to std array
        return vertices;

    }

    auto UnitGrid2DWithColor(const unsigned int divisions) {
        std::vector<float> vertices;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                if(i%2==j%2) {
                    vertices.push_back(1.0);
                    vertices.push_back(1.0);
                    vertices.push_back(1.0);
                    vertices.push_back(1.0);
                    std::cout << xPos << ", " << yPos << ", 0.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
                }else {
                    vertices.push_back(0.0);
                    vertices.push_back(0.0);
                    vertices.push_back(0.0);
                    vertices.push_back(1.0);
                    std::cout << xPos << ", " << yPos << ", 0.0, 0.0, 0.0, 0.0, 1.0" << std::endl;
                }
            }
        }

        //convert grid to std array
        return vertices;
    }

    auto UnitGrid2DTopology(const unsigned int divisions) {

        std::vector<unsigned int> indices;

        // Generate indices for triangles that form the grid
        for (unsigned int i = 0; i < divisions; i++) {
            for (unsigned int j = 0; j < divisions; j++) {
                // Calculate the indices for the four vertices of each quad
                unsigned int topLeft = i * (divisions + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (divisions + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // Define two triangles for each quad
                // Triangle 1: top left -> top right -> bottom left
                indices.push_back(topLeft);
                indices.push_back(topRight);
                indices.push_back(bottomLeft);

                // Triangle 2: top right -> bottom right -> bottom left
                indices.push_back(topRight);
                indices.push_back(bottomRight);
                indices.push_back(bottomLeft);
                //std::cout << topLeft << " " << topRight << " " << bottomLeft << " " << topRight << " " << bottomRight << " " << bottomLeft << std::endl;
            }
        }

        return indices;
    }


}


#endif //PROG2002_GEOMETRICTOOLS_H
