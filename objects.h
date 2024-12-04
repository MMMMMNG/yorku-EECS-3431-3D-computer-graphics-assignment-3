#ifndef OBJECTS_H
#define OBJECTS_H


#include <string>
#include <vector>
#include <glm/vec4.hpp> // glm::dvec4
#include <glm/vec3.hpp> // glm::dvec4

using namespace std;


struct Sphere {
    string name;
    
    // position
    glm::dvec4 pos;
    
    // scale
    glm::dvec4 scale;

    // color doubles [0, 1]
    glm::dvec3 color;

    // between [0,1]
    double Ka;
    double Kd;
    double Ks;
    double Kr;

    // specular exponent of sphere
    int n;

};

struct Light {
    string name;
    
    // position
    glm::dvec4 pos;

    // between [0, 1]
    double Ir;
    double Ig;
    double Ib;
};

struct Scene {
    // Near Plane
    double n;
    double l;
    double r;
    double t;
    double b;

// Resolution
    int x;
    int y;

// Sphere and Light structure arrays
    vector<Sphere> spheres;
    vector<Light> lights;

// Background color as doubles [0, 1]
    double bg_r; 
    double bg_g;
    double bg_b;

// Ambient values as doubles [0, 1]
    double Ir;
    double Ig;
    double Ib;

// Output
    char outputFile[20]; // no spaces or special characters
};

#endif //OBJECTS_H