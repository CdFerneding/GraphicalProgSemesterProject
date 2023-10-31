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

    //according to the issue this function will replace the old UnitGrid2d from now on, but I will keep the ald function

    // Function version of the above
    std::vector<float> UnitGridGeometry2DWTCoords(unsigned int X, unsigned int Y)
    {
        // Implementation is up to you
        return {0.0f};
    }

    // Constexpr version that generates the geometry of the grid, including texture coordinates
    // The shape of the generated data is PPTTPPTTPPTT..., meaning two components for position
    // and two components for texture coordinates.
    auto UnitGrid2DWithTextureCoords(const unsigned int divisions) {
        //Create a std vector of float with size of 3*divisions*divisions
        std::vector<float> vertices;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions)) * 2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions)) * 2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 3D grid

                // Calculate and add texture coordinates
                // these coordinates take the the edges to the area to know where to apply the texture (in our case the chessboard floor)
                // explanation: opengl starts the coordinate grid from the bottom left with 0,0
                // bottom left:
                //if (xPos == -1.0 && yPos == -1.0) {
                //    vertices.push_back(0.0);
                //    vertices.push_back(0.0); 
                //}
                //else if (xPos == 1.0 && yPos == -1.0) { // bottom right corner:
                //    vertices.push_back(1.0);
                //    vertices.push_back(0.0);
                //}
                //else if (xPos == 1.0 && yPos == 1.0) { // bottom top right corner:
                //    vertices.push_back(1.0);
                //    vertices.push_back(1.0);
                //}
                //else if (xPos == -1.0 && yPos == 1.0) { // top left corner:
                //    vertices.push_back(0.0);
                //    vertices.push_back(1.0);
                //}
                //else {
                //    vertices.push_back(0.0);
                //    vertices.push_back(0.0);
                //}
                // 
                // shorter:
                // Calculate and add texture coordinates
                float u = fmod(xPos, 1.0f); // Repeat texture across grid
                float v = fmod(yPos, 1.0f);
                vertices.push_back(u);
                vertices.push_back(v);

            }
        }

        return vertices;
    }


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
        int count = 0;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                vertices.push_back(1.0);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        std::cout << count << std::endl;
        for (int i = 0; i <= divisions; ++i) {
            float xPos = (i / static_cast<float>(divisions))*2 - 1.0f;
            for (int j = 0; j <= divisions; ++j) {
                float yPos = (j / static_cast<float>(divisions))*2 - 1.0f;
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(0.0f); // Set z-coordinate to 0 for a 2D grid
                //put the color in rgba format
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(0.0);
                vertices.push_back(1.0);
                count++;
                //std::cout << xPos << ", " << yPos << ", 1.0, 1.0, 1.0, 1.0, 1.0" << std::endl;
            }
        }
        //convert grid to std array
        return vertices;
    }

    auto UnitGrid2DTopology(const unsigned int divisions) {

        std::vector<unsigned int> indices;

        // Generate indices for triangles that form the grid
        bool isWhite;
        for (unsigned int i = 0; i < divisions; i++) {
            for (unsigned int j = 0; j < divisions; j++) {
                //(divisions+1) * (divisions+1)
                // Calculate the indices for the four vertices of each quad
                isWhite = (i%2==j%2);

                    //It will add 384
                unsigned int topLeft = i * (divisions + 1) + j;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (i + 1) * (divisions + 1) + j;
                unsigned int bottomRight = bottomLeft + 1;

                // Define two triangles for each quad
                // Triangle 1: top left -> top right -> bottom left
                indices.push_back(topLeft+ (isWhite ? 81: 0));
                indices.push_back(topRight+ (isWhite ? 81: 0));
                indices.push_back(bottomLeft+ (isWhite ? 81: 0));

                // Triangle 2: top right -> bottom right -> bottom left
                indices.push_back(topRight+ (isWhite ? 81: 0));
                indices.push_back(bottomRight+ (isWhite ? 81: 0));
                indices.push_back(bottomLeft+ (isWhite ? 81: 0));
                //std::cout << topLeft << " " << topRight << " " << bottomLeft << " " << topRight << " " << bottomRight << " " << bottomLeft << std::endl;
            }
        }

        return indices;
    }

    constexpr std::array<float, 3 * 8> UnitCube = {
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f
    };

    constexpr std::array<float, 8 * 7> UnitCubeWithColor(float r, float g, float b, float a) {
        return {
                -0.5f, -0.5f,  0.0f, r, g, b, a,
                0.5f, -0.5f,  0.0f, r, g, b, a,
                0.5f,  0.5f,  0.0f, r, g, b, a,
                -0.5f,  0.5f,  0.0f, r, g, b, a,
                -0.5f, -0.5f, 1.0f, r, g, b, a,
                0.5f, -0.5f, 1.0f, r, g, b, a,
                0.5f,  0.5f, 1.0f, r, g, b, a,
                -0.5f,  0.5f, 1.0f, r, g, b, a
        };
    }

    constexpr std::array<unsigned int, 36> CubeTopology = {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            4, 7, 3,
            3, 0, 4,
            1, 5, 6,
            6, 2, 1,
            3, 2, 6,
            6, 7, 3,
            0, 1, 5,
            5, 4, 0
    };

}


#endif //PROG2002_GEOMETRICTOOLS_H
