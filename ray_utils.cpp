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


bool nearestIntersection(Ray ray, Sphere sphere, glm::vec4 &nearestIntersection){
    
    glm::mat4 scaleTransform = glm::mat4(
        sphere.scale.x,  0.0, 0.0, 0.0,  // Scale x-axis
        0.0, sphere.scale.y,  0.0, 0.0,  // Scale y-axis
        0.0, 0.0, sphere.scale.z,  0.0,  // Scale z-axis
        sphere.pos.x, sphere.pos.y, sphere.pos.z, 1.0   // Homogeneous coordinate
    );

    glm::mat4 M_inv = glm::inverse(scaleTransform);

    glm::vec3 S_prime = glm::vec3( M_inv * ray.S);
    glm::vec3 c_prime = glm::vec3( M_inv * ray.c);

    //get the coefficients
    float a = glm::dot(c_prime, c_prime);
    float b = 2 * glm::dot(S_prime, c_prime);
    float c = glm::dot(S_prime, S_prime);


    float t1;
    float t2;
    if(!solveQuadratic(a,b,c,t1,t2)){
        return false;
    }

    if(t1 < t2){
        nearestIntersection = evalRay(ray, t1);
    }else{
        nearestIntersection = evalRay(ray, t2);
    }
    return true;
}