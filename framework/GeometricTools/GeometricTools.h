#ifndef PROG2002_GEOMETRICTOOLS_H
#define PROG2002_GEOMETRICTOOLS_H

#include <array>

namespace GeometricTools {
    constexpr std::array<float, 3*2> UnitTriangle2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f}; // [2,3]

    constexpr std::array<float, 3*4> UnitSquare2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f, -0.5f,  0.5f}; // [2,4]

    constexpr std::array<float, 3*4> UnitGridGeometry2D = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f, -0.5f,  0.5f}; // [2,4]

    constexpr std::array<unsigned int, 3*2> UnitGridTopologyTriangles = {0, 1, 2, 0, 2, 3}; // [2,3]


}


#endif //PROG2002_GEOMETRICTOOLS_H
