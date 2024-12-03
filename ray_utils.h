#ifndef RAY_UTILS_H
#define RAY_UTILS_H

#include "objects.h"
#include <glm/vec4.hpp> // glm::vec4

bool solveQuadratic(float a, float b, float c, float &x1, float &x2);
struct Ray {
    glm::vec4 S;  // Starting point (x, y, z)
    glm::vec4 c;  // Direction vector (dx, dy, dz)
};

struct Hit {
    const Ray* ray;        // Pointer to the ray
    const Sphere* sphere;  // Pointer to the sphere
    float t;               // Parameter t for the hit point

    Hit() : ray(nullptr), sphere(nullptr), t(0.0f) {} // Default constructor
};

bool nearestIntersection(Ray ray, Sphere sphere, float &nearest_t);
bool findNearestHitWithAllObjects(Ray &ray, Scene &scene, Hit &nearestHit);
bool findAnyHitWithAllObjects(Ray &ray, Scene &scene);


#endif //RAY_UTILS_H