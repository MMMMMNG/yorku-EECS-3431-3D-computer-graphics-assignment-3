#ifndef RAY_UTILS_H
#define RAY_UTILS_H

#include "objects.h"
#include <glm/vec4.hpp> // glm::vec4

bool solveQuadratic(float a, float b, float c, float &x1, float &x2);
struct Ray {
    glm::vec4 S;  // Starting point (x, y, z)
    glm::vec4 c;  // Direction vector (dx, dy, dz)
};

glm::vec4 nearestIntersection(Ray ray, Sphere sphere);


#endif //RAY_UTILS_H