#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

typedef struct {
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

} Sphere;

typedef struct {
    string name;
    
    // position
    float posx;
    float posy;
    float posz;

    // between [0, 1]
    float Ir;
    float Ig;
    float Ib;
} Light;

// Near Plane
    float n;
    float l;
    float r;
    float t;
    float b;

// Resolution
    int x;
    int y;

// TO ADD HERE: create sphere and light objects w attributes passed in
    Sphere sphere;
    Light light;

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

    // make sure file is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the file for reading
    string filename = argv[1];
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    // read values from the file
    // smtg like sphere.sclx = inputFile >> (data);

    // close file
    inputFile.close();

    return 0;
}