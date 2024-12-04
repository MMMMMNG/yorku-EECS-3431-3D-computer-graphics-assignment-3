#ifndef RAY_UTILS_H
#define RAY_UTILS_H

#include "objects.h"
#include <glm/vec4.hpp> // glm::vec4

bool solveQuadratic(double a, double b, double c, double &x1, double &x2);
struct Ray {
    glm::dvec4 S;  // Starting point (x, y, z)
    glm::dvec4 c;  // Direction vector (dx, dy, dz)
};

struct Hit {
    const Ray* ray;        // Pointer to the ray
    const Sphere* sphere;  // Pointer to the sphere
    double t;               // Parameter t for the hit point
    glm::dvec3 normal;

    Hit() : ray(nullptr), sphere(nullptr), t(0.0f), normal(0.0f, 0.0f, 0.0f) {} // Default constructor
};

bool nearestIntersection(Ray ray, Sphere sphere, double &nearest_t);
bool findNearestHitWithAllObjects(Ray &ray, Scene &scene, Hit &nearestHit);
bool findAnyHitWithAllObjects(Ray &ray, Scene &scene);
void rayTraceAllPixels(const Scene &scene, unsigned char* pixels);


#endif //RAY_UTILS_H