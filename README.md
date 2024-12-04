# EECS 3431 - 3D computer graphics Fall 2024 York University, Toronto
Assignment 3: Raytracer

Jessica Corkal & Marius Gebhardt

Email: jcorkal@my.yorku.ca Student ID: 219314095

Email: mmmm@my.yorku.ca Student ID: 221828934

We were able to implement a generic raytracer that generates a ppm image. The program reads in a text file and calculates ray-sphere intersections to draw spheres based on the specified location, scale, and color. It takes into account scene background, lighting, and attempts to render reflection and refraction.

---------- Learning Resources ----------------
Phong Illumination Model explanation: http://rodolphe-vaillant.fr/entry/85/phong-illumination-model-cheat-sheet

Setting up Windows Subsystem Linux environment for VSCode: https://code.visualstudio.com/docs/cpp/config-wsl

Understanding Raytracing algorithm: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/shadingwithsurfacenormals

Understanding Reflection and Refraction: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction.html 

---------- Libraries ----------------
glm: https://github.com/g-truc/glm