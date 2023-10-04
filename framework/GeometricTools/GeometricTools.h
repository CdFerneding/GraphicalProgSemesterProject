#ifndef PROG2002_GEOMETRICTOOLS_H
#define PROG2002_GEOMETRICTOOLS_H

#include <array>

namespace GeometricTools {
    constexpr std::array<float, 3*2> UnitTriangle2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f}; // [2,3]

    constexpr std::array<float, 3*4> UnitSquare2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f, -0.5f,  0.5f}; // [2,4]

    float * UnitGrid2D(const unsigned int divisions) {
        auto grid = new float[3*(divisions+1)*(divisions+1)];
        for (unsigned int i = 0; i <= divisions; i++) {
            for (unsigned int j = 0; j <= divisions; j++) {
                grid[3*(i*(divisions+1)+j)] = -0.5f + (float)j/divisions;
                grid[3*(i*(divisions+1)+j)+1] = -0.5f + (float)i/divisions;
                grid[3*(i*(divisions+1)+j)+2] = 0.0f;
            }
        }
        return grid;
    };

    unsigned int * UnitGrid2DTopology(const unsigned int divisions) {
        auto * topology = new unsigned int[3*2*divisions*divisions];
        for (unsigned int i = 0; i < divisions; i++) {
            for (unsigned int j = 0; j < divisions; j++) {
                topology[6*(i*divisions+j)] = i*(divisions+1)+j;
                topology[6*(i*divisions+j)+1] = i*(divisions+1)+j+1;
                topology[6*(i*divisions+j)+2] = (i+1)*(divisions+1)+j+1;
                topology[6*(i*divisions+j)+3] = i*(divisions+1)+j;
                topology[6*(i*divisions+j)+4] = (i+1)*(divisions+1)+j+1;
                topology[6*(i*divisions+j)+5] = (i+1)*(divisions+1)+j;
            }
        }
        return topology;
    };

}


#endif //PROG2002_GEOMETRICTOOLS_H
