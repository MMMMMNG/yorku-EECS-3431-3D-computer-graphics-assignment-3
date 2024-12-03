#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "ray_utils.h"

using namespace std;

/* camera is fixed at origin (right hand cs looking down -z)

TO IMPLEMENT:
- local illumination
- reflections
- shadows

Define variables to store raytracer data
Syntax:
NEAR <n>
LEFT <l>
RIGHT <r>
BOTTOM <b>
TOP <t>
RES <x> <y>
SPHERE <name> <pos x> <pos y> <pos z> <scl x> <scl y> <scl z> <r> <g> <b> <Ka> <Kd> <Ks> <Kr> <n>
… // up to 14 additional sphere specifications
LIGHT <name> <pos x> <pos y> <pos z> <Ir> <Ig> <Ib>
… // up to 9 additional light specifications
BACK <r> <g > <b>
AMBIENT <Ir> <Ig> <Ib>
OUTPUT <name>
*/

struct Sphere {
    string name;
    
    // position
    float posx;
    float posy;
    float posz;
    
    // scale
    float sclx;
    float scly;
    float sclz;

    // color floats [0, 1]
    float r;
    float g;
    float b;

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

int main(int argc, char *argv[]) {

    //poc vector math
    glm::vec3 one = glm::vec3(1,2,3);
    glm::vec3 two = glm::vec3(3,2,1);
    glm::vec3 out = one + two;
    cout << "out x: "<< out.x << " y: " << out.y << " z: " << out.z;

    double a = 1, b = -3, c = 2;  // Example coefficients
    double x1, x2;

    if (solveQuadratic(a, b, c, x1, x2)) {
        std::cout << "Solutions exist: x1 = " << x1 << ", x2 = " << x2 << "\n";
    } else {
        std::cout << "No real solutions exist.\n";
    }

    // make sure file is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the file and read
    string filename = argv[1];
    ifstream inputFile(filename);

    // check for errors opening
    if (!inputFile) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    // read values from the file
    string line;

    while(getline(inputFile, line)) {
        // store values
        istringstream stream(line); // used to go through each word in line
        string firstWord;

        stream >> firstWord;

        if(firstWord.compare("NEAR")) {
            stream >> n;
        }

        if(firstWord.compare("LEFT")) {
            stream >> l;
        }

        if(firstWord.compare("RIGHT")) {
            stream >> r;
        }

        if(firstWord.compare("BOTTOM")) {
            stream >> b;
        }

        if(firstWord.compare("TOP")) {
            stream >> t;
        }

        if(firstWord.compare("RES")) {
            stream >> x;
            stream >> y;
        }
        
        if(firstWord.compare("SPHERE")) {
            Sphere s = {};

            stream >> s.name;

            stream >> s.posx;
            stream >> s.posy;
            stream >> s.posz;

            stream >> s.sclx;
            stream >> s.scly;
            stream >> s.sclz;

            stream >> s.r;
            stream >> s.g;
            stream >> s.b;

            stream >> s.Ka;
            stream >> s.Kd;
            stream >> s.Ks;
            stream >> s.Kr;

            stream >> s.n;

            spheres.push_back(s);
        }

        if(firstWord.compare("LIGHT")) {
            Light li = {};

            stream >> li.name;

            stream >> li.posx;
            stream >> li.posy;
            stream >> li.posz;

            stream >> li.Ir;
            stream >> li.Ig;
            stream >> li.Ib;

            lights.push_back(li);
        }

        if(firstWord.compare("BACK")) {
            stream >> bg_r;
            stream >> bg_g;
            stream >> bg_b;
        }

        if(firstWord.compare("AMBIENT")) {
            stream >> Ir;
            stream >> Ig;
            stream >> Ib;
        }

        if(firstWord.compare("OUTPUT")) {
            stream >> outputFile;
        }
    }

    // close file
    inputFile.close();

    return 0;
}