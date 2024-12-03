#ifndef OBJECTS_H
#define OBJECTS_H


#include <string>
#include <vector>
#include <glm/vec4.hpp> // glm::vec4
#include <glm/vec3.hpp> // glm::vec4

using namespace std;


struct Sphere {
    string name;
    
    // position
    glm::vec4 pos;
    
    // scale
    glm::vec4 scale;

    // color floats [0, 1]
    glm::vec3 color;

    // between [0,1]
    float Ka;
    float Kd;
    float Ks;
    float Kr;

    // specular exponent of sphere
    int n;

};

struct Light {
    string name;
    
    // position
    float posx;
    float posy;
    float posz;

    // between [0, 1]
    float Ir;
    float Ig;
    float Ib;
};

struct Scene {
    // Near Plane
    float n;
    float l;
    float r;
    float t;
    float b;

// Resolution
    int x;
    int y;

// Sphere and Light structure arrays
    vector<Sphere> spheres;
    vector<Light> lights;

// Background color as floats [0, 1]
    float bg_r; 
    float bg_g;
    float bg_b;

// Ambient values as floats [0, 1]
    float Ir;
    float Ig;
    float Ib;

// Output
    string outputFile; // no spaces or special characters
};

#endif //OBJECTS_H