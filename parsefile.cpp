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

        if(firstWord == "NEAR") {
            stream >> scene.n;
        }

        if(firstWord == "LEFT") {
            stream >> scene.l;
        }

        if(firstWord == "RIGHT") {
            stream >> scene.r;
        }

        if(firstWord == "BOTTOM") {
            stream >> scene.b;
        }

        if(firstWord == "TOP") {
            stream >> scene.t;
        }

        if(firstWord == "RES") {
            stream >> scene.x;
            stream >> scene.y;
        }
        
        if(firstWord == "SPHERE") {
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

        if(firstWord == "LIGHT") {
            Light li = {};

            stream >> li.name;

            stream >> li.pos.x;
            stream >> li.pos.y;
            stream >> li.pos.z;
            li.pos.w = 1.0f;

            stream >> li.Ir;
            stream >> li.Ig;
            stream >> li.Ib;

            scene.lights.push_back(li);
        }

        if(firstWord == "BACK") {
            stream >> scene.bg_r;
            stream >> scene.bg_g;
            stream >> scene.bg_b;
        }

        if(firstWord == "AMBIENT") {
            stream >> scene.Ir;
            stream >> scene.Ig;
            stream >> scene.Ib;
        }

        if(firstWord == "OUTPUT") {
            stream >> scene.outputFile;
        }
    }

    // close file
    inputFile.close();

    return 0;
}