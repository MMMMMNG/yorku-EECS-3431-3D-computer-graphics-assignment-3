#include <stdio.h>
#include <stdlib.h>

// Define variables to store raytracer data
    /* Syntax:
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
    OUTPUT <name>*/

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

// Background color as floats [0, 1]
    float r; 
    float g;
    float b;

// Ambient values as floats [0, 1]
    float Ir;
    float Ig;
    float Ib;

// Output
    char outputFile[20]; // no spaces or special characters

int main(int argc, char *argv[]) {
    // make sure file is provided
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file. Exiting program...");
        return 1;
    }

    // read values from the file
    //smtg like fscanf(file, "%s %f %d", outputFile, &n, &x) but in order of?

    // close file
    fclose(file);

    return 0;
}