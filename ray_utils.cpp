#include "ray_utils.h"
#include <iostream>
#include <cmath>          // For std::sqrt
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>
#include <glm/gtx/component_wise.hpp>

#define MAX_DEPTH 3

bool solveQuadratic(float a, float b, float c, float &x1, float &x2)
{
    if (a == 0)
    { // Degenerate case: Not a quadratic equation
        if (b == 0)
        {
            return false; // No solution if a and b are 0
        }
        x1 = x2 = -c / b; // Linear solution
        return true;
    }

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
    {
        return false; // No real solutions
    }
    else if (discriminant == 0)
    {
        x1 = x2 = -b / (2 * a); // One real solution
    }
    else
    {
        float sqrtDiscriminant = std::sqrt(discriminant);
        x1 = (-b + sqrtDiscriminant) / (2 * a);
        x2 = (-b - sqrtDiscriminant) / (2 * a);
    }

    return true;
}

glm::vec4 evalRay(Ray ray, float t)
{
    return ray.S + t * ray.c;
}

bool nearestIntersection(Ray ray, Sphere sphere, float &nearest_t)
{
    // Create the transformation matrix
    glm::mat4 scaleTransform = glm::mat4(
        sphere.scale.x, 0.0, 0.0, 0.0,                // Scale x-axis
        0.0, sphere.scale.y, 0.0, 0.0,                // Scale y-axis
        0.0, 0.0, sphere.scale.z, 0.0,                // Scale z-axis
        sphere.pos.x, sphere.pos.y, sphere.pos.z, 1.0 // Translation
    );

    glm::mat4 M_inv = glm::inverse(scaleTransform);

    // Transform ray starting point (divide by w for homogeneous coordinate)
    glm::vec4 S_trans = M_inv * ray.S;
    glm::vec3 S_prime = glm::vec3(S_trans); // / S_trans.w;

    // Transform ray direction (keep homogeneous w = 0)
    glm::vec4 c_trans = M_inv * ray.c;
    glm::vec3 c_prime = glm::vec3(c_trans);

    // Compute quadratic coefficients
    float a = glm::dot(c_prime, c_prime);
    float b = 2.0f * glm::dot(S_prime, c_prime);
    float c = glm::dot(S_prime, S_prime) - 1.0f; // Sphere radius = 1 in transformed space

    // Solve the quadratic equation
    float t1, t2;
    if (!solveQuadratic(a, b, c, t1, t2))
    {
        return false; // No intersection
    }

    // Find the nearest valid t
    if (t1 > 0.0f && t2 > 0.0f)
    {
        nearest_t = glm::min(t1, t2);
    }
    else if (t1 > 0.0f)
    {
        nearest_t = t1;
    }
    else if (t2 > 0.0f)
    {
        nearest_t = t2;
    }
    else
    {
        return false; // Both intersections are behind the ray's starting point
    }

    return true;
}

bool findNearestHitWithAllObjects(Ray &ray, const Scene &scene, Hit &nearestHit, float minimum_t)
{
    float closest_t = std::numeric_limits<float>::max(); // Start with a large value
    bool hitFound = false;

    for (const Sphere &sphere : scene.spheres)
    {
        float t;
        if (nearestIntersection(ray, sphere, t))
        {
            if (t < closest_t && t > minimum_t)
            { // Update only if t is closer and positive
                closest_t = t;
                nearestHit.ray = &ray;
                nearestHit.sphere = &sphere;
                nearestHit.t = t;
                hitFound = true;
            }
        }
    }

    return hitFound;
}

bool findAnyHitWithAllObjects(Ray &ray, const Scene &scene)
{
    for (const Sphere &sphere : scene.spheres)
    {
        float t;
        if (nearestIntersection(ray, sphere, t))
        {
            return true; // in shadow
        }
    }
    return false; // no shadow
}

bool findAnyHitWithAllObjectsBetweenLightAndObject(Ray &ray, const Scene &scene, float length)
{
    for (const Sphere &sphere : scene.spheres)
    {
        float t;
        if (nearestIntersection(ray, sphere, t))
        {
            if (t > 0.000001f && t < length)
            {                // we only care if it's between the obj & light
                return true; // in shadow
            }
        }
    }
    return false; // no shadow
}

glm::vec3 shootShadowRays(const glm::vec3 &point, const Scene &scene)
{
    glm::vec3 colorInfluence(0.0f, 0.0f, 0.0f);

    // Loop through all lights in the scene
    for (const Light &light : scene.lights)
    {
        // Compute the direction from the point to the light
        glm::vec3 lightDir(light.pos.x - point.x, light.pos.y - point.y, light.pos.z - point.z);
        float length = glm::length(lightDir);
        glm::vec3 lightDirNormalized = lightDir / length;

        // Create a ray from the point towards the light
        Ray shadowRay;
        shadowRay.S = glm::vec4(point, 1.0f);
        shadowRay.c = glm::vec4(lightDir, 0.0f);

        // Check if the point is in shadow
        if (!findAnyHitWithAllObjectsBetweenLightAndObject(shadowRay, scene, length))
        {
            // If no objects are blocking the light, add the light's influence to the color
            float lightStrength = glm::dot(lightDir, lightDir); // Intensity proportional to the distance
            glm::vec3 lightColor(light.Ir, light.Ig, light.Ib);
            colorInfluence += (lightColor / lightStrength); // Accumulate light influence
        }
    }

    return colorInfluence / glm::compMax(colorInfluence);
}


// s is the index of sphere
glm::vec3 computeLighting(glm::vec4 pixelPos, glm::vec4 direction, const Scene &scene, Sphere currentSphere)
{
    // Check for intersection here (not included in this snippet)

    glm::vec3 normal = glm::normalize(glm::vec3(direction)); 
    glm::vec3 viewingDirection(0, 0, 1); // Camera at fixed location (assuming camera is at origin along z-axis)

    glm::vec3 ambient(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuse(0.0f, 0.0f, 0.0f);
    glm::vec3 specular(0.0f, 0.0f, 0.0f);

    // Calculate for each light in the scene
    for(int i = 0; i < scene.lights.size(); i++) {
        // Ambient 
        ambient += glm::vec3(scene.lights.at(i).Ir, scene.lights.at(i).Ig, scene.lights.at(i).Ib) * currentSphere.Ka * currentSphere.color;

        // Directional 
        glm::vec3 lightDir = glm::normalize(glm::vec3(scene.lights.at(i).pos - pixelPos)); 
        float diff = glm::max(glm::dot(normal, lightDir), 0.0f); 
        diffuse += diff * currentSphere.Kd * glm::vec3(scene.lights.at(i).Ir, scene.lights.at(i).Ig, scene.lights.at(i).Ib) * currentSphere.color;
    
        // Specular
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal); // Reflection of the light direction around the normal
        float spec = glm::pow(glm::max(glm::dot(viewingDirection, reflectDir), 0.0f), currentSphere.n); // Specular term based on camera angle
        specular += spec * currentSphere.Ks * currentSphere.Kr * glm::vec3(scene.lights.at(i).Ir, scene.lights.at(i).Ig, scene.lights.at(i).Ib) * currentSphere.color;
    }

    glm::vec3 combinedColor = ambient + diffuse + specular;

    return combinedColor;
}
// the meat of the program
glm::vec3 raytrace(int depth, Ray &ray, const Scene &scene)
{
    if (depth > MAX_DEPTH)
    {
        return glm::vec3(0, 0, 0); // we stop after recursion depth 4
    }
    Hit hit;
    if (!findNearestHitWithAllObjects(ray, scene, hit, 1.0f))
    {
        return glm::vec3(scene.bg_r, scene.bg_g, scene.bg_b); // return background color if the ray doesn't hit anything
    }

    // get intersection point
    glm::vec3 point = evalRay(ray, hit.t);

    // shoot shadow rays
    //glm::vec3 c_local = shootShadowRays(point, scene);

    //return c_local * hit.sphere->color;
    return computeLighting(ray.S + ray.c, ray.c, scene, *hit.sphere);
}


void rayTraceAllPixels(const Scene &scene, unsigned char *pixels)
{
    // Origin of the camera
    glm::vec4 origin(0.0f, 0.0f, 0.0f, 1.0f);

    // Step sizes for each pixel on the near plane
    float dx = (scene.r - scene.l) / scene.x;
    float dy = (scene.t - scene.b) / scene.y;

    for (int j = 0; j < scene.y; ++j)
    {
        for (int i = 0; i < scene.x; ++i)
        {
            // Compute the pixel position on the near plane
            float px = scene.l + (i + 0.5f) * dx; // Center of the pixel in X
            float py = scene.b + (j + 0.5f) * dy; // Center of the pixel in Y (flip Y-axis)
            float pz = -scene.n;                  // Near plane is at -n in camera space

            // Define the ray
            glm::vec4 pixelPos(px, py, pz, 1.0f);
            glm::vec4 direction = pixelPos - origin; // Normalize not necessary. This also allows us to exclude intersections that
                                                     // are in front of the nearplane (t < 1)
            Ray ray = Ray{origin, glm::vec4(direction.x, direction.y, direction.z, 0.0f)};

            // Determine pixel color
            glm::vec3 color = raytrace(1, ray, scene);

            int pixOffset = 3 * (i + j * scene.x);
           
            pixels[pixOffset] = color.x * 255;
            pixels[pixOffset + 1] = color.y * 255;
            pixels[pixOffset + 2] = color.z * 255;
        }
    }
}