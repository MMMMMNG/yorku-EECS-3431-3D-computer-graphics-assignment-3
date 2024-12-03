#include "ray_utils.h"

#include <cmath>  // For std::sqrt
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>

bool solveQuadratic(float a, float b, float c, float &x1, float &x2) {
    if (a == 0) {  // Degenerate case: Not a quadratic equation
        if (b == 0) {
            return false;  // No solution if a and b are 0
        }
        x1 = x2 = -c / b;  // Linear solution
        return true;
    }

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;  // No real solutions
    } else if (discriminant == 0) {
        x1 = x2 = -b / (2 * a);  // One real solution
    } else {
        float sqrtDiscriminant = std::sqrt(discriminant);
        x1 = (-b + sqrtDiscriminant) / (2 * a);
        x2 = (-b - sqrtDiscriminant) / (2 * a);
    }

    return true;
}

glm::vec4 evalRay(Ray ray, float t){
    return ray.S + t * ray.c;
}


bool nearestIntersection(Ray ray, Sphere sphere, float &nearest_t) {
    // Create the transformation matrix
    glm::mat4 scaleTransform = glm::mat4(
        sphere.scale.x,  0.0, 0.0, 0.0,  // Scale x-axis
        0.0, sphere.scale.y,  0.0, 0.0,  // Scale y-axis
        0.0, 0.0, sphere.scale.z,  0.0,  // Scale z-axis
        sphere.pos.x, sphere.pos.y, sphere.pos.z, 1.0   // Translation
    );

    glm::mat4 M_inv = glm::inverse(scaleTransform);

    // Transform ray starting point (divide by w for homogeneous coordinate)
    glm::vec4 S_trans = M_inv * ray.S;
    glm::vec3 S_prime = glm::vec3(S_trans) / S_trans.w;

    // Transform ray direction (keep homogeneous w = 0)
    glm::vec4 c_trans = M_inv * ray.c;
    glm::vec3 c_prime = glm::vec3(c_trans);

    // Compute quadratic coefficients
    float a = glm::dot(c_prime, c_prime);
    float b = 2.0f * glm::dot(S_prime, c_prime);
    float c = glm::dot(S_prime, S_prime) - 1.0f; // Sphere radius = 1 in transformed space

    // Solve the quadratic equation
    float t1, t2;
    if (!solveQuadratic(a, b, c, t1, t2)) {
        return false; // No intersection
    }

    // Find the nearest valid t
    if (t1 > 0.0f && t2 > 0.0f) {
        nearest_t = glm::min(t1, t2);
    } else if (t1 > 0.0f) {
        nearest_t = t1;
    } else if (t2 > 0.0f) {
        nearest_t = t2;
    } else {
        return false; // Both intersections are behind the ray's starting point
    }

    return true;
}

bool findNearestHitWithAllObjects(Ray &ray, Scene &scene, Hit &nearestHit) {
    float closest_t = std::numeric_limits<float>::max(); // Start with a large value
    bool hitFound = false;

    for (const Sphere &sphere : scene.spheres) {
        float t;
        if (nearestIntersection(ray, sphere, t)) {
            if (t < closest_t && t > 0.0f) { // Update only if t is closer and positive
                closest_t = t;
                nearestHit.ray = &ray;
                nearestHit.sphere = &sphere;
                nearestHit.t = t;
                hitFound = true;
            }
        }
    }

    return hitFound;
}

bool findAnyHitWithAllObjects(Ray &ray, const Scene &scene){
    for (const Sphere &sphere : scene.spheres) {
        float t;
        if (nearestIntersection(ray, sphere, t)) {
            return true; // in shadow
        }
    }
    return false; // no shadow
}


void rayTraceAllPixels(const Scene &scene, unsigned char* pixels) {
    // Origin of the camera
    glm::vec4 origin(0.0f, 0.0f, 0.0f, 1.0f);

    // Step sizes for each pixel on the near plane
    float dx = (scene.r - scene.l) / scene.x;
    float dy = (scene.t - scene.b) / scene.y;

    for (int j = 0; j < scene.y; ++j) {
        for (int i = 0; i < scene.x; ++i) {
            // Compute the pixel position on the near plane
            float px = scene.l + (i + 0.5f) * dx;  // Center of the pixel in X
            float py = scene.b + (j + 0.5f) * dy;  // Center of the pixel in Y (flip Y-axis)
            float pz = -scene.n;                   // Near plane is at -n in camera space

            // Define the ray
            glm::vec4 pixelPos(px, py, pz, 1.0f);
            glm::vec4 direction = glm::normalize(pixelPos - origin); // Normalize direction
            Ray ray = Ray{origin, glm::vec4(direction.x, direction.y, direction.z, 0.0f)};

            // Determine pixel color
            int pixOffset = 3 * (i + j * scene.x);
            if (findAnyHitWithAllObjects(ray, scene)) {
                pixels[pixOffset] = 0;
                pixels[pixOffset + 1] = 0;
                pixels[pixOffset + 2] = 0;
            } else {
                pixels[pixOffset] = 255;
                pixels[pixOffset + 1] = 255;
                pixels[pixOffset + 2] = 255;
            }
        }
    }
}

glm::vec3 computeLighting(Scene scene) {
    // check for intersection 
    glm::vec3 normal (0, 0, 1);

    int i = 0;

    float ambientR = scene.Ir * scene.spheres.at(i).Ka;
    float ambientG = scene.Ig * scene.spheres.at(i).Ka;
    float ambientB = scene.Ib * scene.spheres.at(i).Ka;

    // for each light
    float totalDiffuse;

    for(int i = 0; i < scene.lights.size(); i++) {
        // float totalDiffuse += (normal * scene.lights.at(i).posx) * scene.spheres.at(i).Kd;
    }

    float specularR;
    float specularG;
    float specularB;

    float lightR = ambientR + totalDiffuse + specularR;
    float lightG = ambientG + totalDiffuse + specularB;
    float lightB = ambientB + totalDiffuse + specularB;

    glm::vec3 combinedColor (lightR, lightG, lightB);

    return combinedColor;
}