#ifndef RAY_UTILS_H
#define RAY_UTILS_H


#include <glm/vec4.hpp> // glm::vec4

bool solveQuadratic(double a, double b, double c, double &x1, double &x2);
struct Ray {
    glm::vec4 S;  // Starting point (x, y, z)
    glm::vec4 c;  // Direction vector (dx, dy, dz)
};


#endif //RAY_UTILS_H