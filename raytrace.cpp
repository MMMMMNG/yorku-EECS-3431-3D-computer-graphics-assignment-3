#include <iostream>
#include <string>
#include <vector>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include "ray_utils.h"
#include "parsefile.h"
#include "objects.h"
#include "ppm.h"

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



int main(int argc, char *argv[]) {
    // make sure file is provided
    // if (argc != 2) {
    //     cerr << "Usage: " << argv[0] << " <filename>" << endl;
    //     return 1;
    // }

    // Open the file and read
    string filename = "/mnt/c/Users/jcork/Documents/School/Year3Courses/EECS3431/yorku-EECS-3431-3D-computer-graphics-assignment-3/testAmbient.txt" ; //argv[1]
    Scene scene;

    if(parsefile(filename, scene) > 0){
        return 1;
    }

    // set camera and image plane
    unsigned char *pixels;
    pixels = new unsigned char [3*scene.x*scene.y];

    rayTraceAllPixels(scene, pixels);
    
    save_imageP6(scene.x, scene.y, scene.outputFile, pixels);
    return 0;
}