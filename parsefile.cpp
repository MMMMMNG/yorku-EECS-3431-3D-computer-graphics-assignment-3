#include "parsefile.h"
#include <fstream>
#include <sstream>
#include "objects.h"
#include <string>
#include <iostream>

using namespace std;

int parsefile(string filename, Scene &scene){
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
            stream >> scene.n;
        }

        if(firstWord.compare("LEFT")) {
            stream >> scene.l;
        }

        if(firstWord.compare("RIGHT")) {
            stream >> scene.r;
        }

        if(firstWord.compare("BOTTOM")) {
            stream >> scene.b;
        }

        if(firstWord.compare("TOP")) {
            stream >> scene.t;
        }

        if(firstWord.compare("RES")) {
            stream >> scene.x;
            stream >> scene.y;
        }
        
        if(firstWord.compare("SPHERE")) {
            Sphere s;

            stream >> s.name;

            stream >> s.pos.x;
            stream >> s.pos.y;
            stream >> s.pos.z;

            stream >> s.scale.x;
            stream >> s.scale.y;
            stream >> s.scale.z;

            stream >> s.color.x;
            stream >> s.color.y;
            stream >> s.color.z;

            stream >> s.Ka;
            stream >> s.Kd;
            stream >> s.Ks;
            stream >> s.Kr;

            stream >> s.n;

            scene.spheres.push_back(s);
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

            scene.lights.push_back(li);
        }

        if(firstWord.compare("BACK")) {
            stream >> scene.bg_r;
            stream >> scene.bg_g;
            stream >> scene.bg_b;
        }

        if(firstWord.compare("AMBIENT")) {
            stream >> scene.Ir;
            stream >> scene.Ig;
            stream >> scene.Ib;
        }

        if(firstWord.compare("OUTPUT")) {
            stream >> scene.outputFile;
        }
    }

    // close file
    inputFile.close();
    
    return 0;
}