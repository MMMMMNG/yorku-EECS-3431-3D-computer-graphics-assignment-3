#include "ray_utils.h"
#include <iostream>
#include <cmath>          // For std::sqrt
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/vec3.hpp>
#include <glm/gtx/component_wise.hpp>

#define MAX_DEPTH 3

bool solveQuadratic(double a, double b, double c, double &x1, double &x2)
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

    double discriminant = b * b - 4 * a * c;

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
        double sqrtDiscriminant = std::sqrt(discriminant);
        x1 = (-b + sqrtDiscriminant) / (2 * a);
        x2 = (-b - sqrtDiscriminant) / (2 * a);
    }

    return true;
}

glm::dvec4 evalRay(Ray ray, double t)
{
    return ray.S + t * ray.c;
}

bool nearestIntersection(Ray ray, Sphere sphere, double &nearest_t)
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
    glm::dvec4 S_trans = M_inv * ray.S;
    glm::dvec3 S_prime = glm::dvec3(S_trans); // / S_trans.w;

    // Transform ray direction (keep homogeneous w = 0)
    glm::dvec4 c_trans = M_inv * ray.c;
    glm::dvec3 c_prime = glm::dvec3(c_trans);

    // Compute quadratic coefficients
    double a = glm::dot(c_prime, c_prime);
    double b = 2.0f * glm::dot(S_prime, c_prime);
    double c = glm::dot(S_prime, S_prime) - 1.0f; // Sphere radius = 1 in transformed space

    // Solve the quadratic equation
    double t1, t2;
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

bool nearestIntersectionWithNormal(Ray ray, Sphere sphere, double &nearest_t, glm::dvec3 &normal_at_intersection)
{
    // Create the transformation matrix
    glm::mat4 scaleTransform = glm::dmat4(
        sphere.scale.x, 0.0, 0.0, 0.0,                // Scale x-axis
        0.0, sphere.scale.y, 0.0, 0.0,                // Scale y-axis
        0.0, 0.0, sphere.scale.z, 0.0,                // Scale z-axis
        sphere.pos.x, sphere.pos.y, sphere.pos.z, 1.0 // Translation
    );

    glm::mat4 M_inv = glm::inverse(scaleTransform);
    glm::mat4 M_inv_transpose = glm::transpose(M_inv);

    // Transform ray starting point (divide by w for homogeneous coordinate)
    glm::dvec4 S_trans = M_inv * ray.S;
    glm::dvec3 S_prime = glm::dvec3(S_trans); // / S_trans.w;

    // Transform ray direction (keep homogeneous w = 0)
    glm::dvec4 c_trans = M_inv * ray.c;
    glm::dvec3 c_prime = glm::dvec3(c_trans);

    // Compute quadratic coefficients
    double a = glm::dot(c_prime, c_prime);
    double b = 2.0f * glm::dot(S_prime, c_prime);
    double c = glm::dot(S_prime, S_prime) - 1.0f; // Sphere radius = 1 in transformed space

    // Solve the quadratic equation
    double t1, t2;
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

    // Compute the intersection point in the transformed space
    glm::dvec3 intersection_local = S_prime + nearest_t * c_prime;

    // Compute the normal in the local space of the sphere
    glm::dvec3 normal_local = glm::normalize(intersection_local);

    // Transform the normal to world space using the inverse transpose matrix
    glm::dvec3 normal_world = glm::normalize(glm::dvec3(M_inv_transpose * glm::dvec4(normal_local, 0.0f)));

    // Set the output parameter
    normal_at_intersection = normal_world;

    return true;
}

bool findNearestHitWithAllObjects(Ray &ray, const Scene &scene, Hit &nearestHit, double minimum_t)
{
    double closest_t = std::numeric_limits<double>::max(); // Start with a large value
    bool hitFound = false;

    for (const Sphere &sphere : scene.spheres)
    {
        double t;
        glm::dvec3 normal;
        if (nearestIntersectionWithNormal(ray, sphere, t, normal))
        {
            if (t < closest_t && t > minimum_t)
            { // Update only if t is closer and positive
                closest_t = t;
                nearestHit.ray = &ray;
                nearestHit.sphere = &sphere;
                nearestHit.t = t;
                nearestHit.normal = normal;
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
        double t;
        if (nearestIntersection(ray, sphere, t))
        {
            return true; // in shadow
        }
    }
    return false; // no shadow
}

bool findAnyHitWithAllObjectsBetweenLightAndObject(Ray &ray, const Scene &scene, double length)
{
    for (const Sphere &sphere : scene.spheres)
    {
        double t;
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

glm::dvec3 computeLighting(glm::dvec3 rayDir, glm::dvec3 normal, glm::dvec3 pos, const Light &light, glm::dvec3 lightDir, Sphere currentSphere)
{
    // Check for intersection here (not included in this snippet)

    glm::dvec3 viewingDirection(0, 0, -1); // Camera at fixed location (assuming camera is at origin along z-axis)

    glm::dvec3 lightColor = glm::dvec3(light.Ir, light.Ig, light.Ib);

    // Directional
    double diff = glm::max(glm::dot(normal, lightDir), 0.0);
    glm::dvec3 diffuse = diff * currentSphere.Kd * lightColor * currentSphere.color;

    // Specular
    glm::dvec3 reflectDir = glm::reflect(lightDir, normal);                                        // Reflection of the light direction around the normal
    double spec = glm::pow(glm::dot(reflectDir, rayDir), currentSphere.n); // Specular term based on camera angle
    glm::dvec3 specular = spec * currentSphere.Ks * lightColor;

    return diffuse + specular;
}

glm::dvec3 shootShadowRays(const Hit &hit, glm::dvec3 point, const Scene &scene, const Ray ray, Sphere sphere)
{
    glm::dvec3 colorInfluence(0.0f, 0.0f, 0.0f);

    // Loop through all lights in the scene
    for (const Light &light : scene.lights)
    {
        // Compute the direction from the point to the light
        glm::dvec3 lightDir = glm::dvec3(light.pos) - point;
        double length = glm::length(lightDir);
        glm::dvec3 lightDirNormalized = glm::normalize(lightDir);// / length;

        // Create a ray from the point towards the light
        Ray shadowRay;
        shadowRay.S = glm::dvec4(point, 1.0f);
        shadowRay.c = glm::dvec4(lightDirNormalized, 0.0f);

        // Check if the point is in shadow
        if (!findAnyHitWithAllObjectsBetweenLightAndObject(shadowRay, scene, length))
        {
            colorInfluence += computeLighting(ray.c, hit.normal, point, light, lightDirNormalized, sphere);
        }
    }

    return glm::clamp(colorInfluence, 0.0, 1.0);
}
// the meat of the program
glm::dvec3 raytrace(int depth, Ray &ray, const Scene &scene)
{
    if (depth > MAX_DEPTH)
    {
        return glm::dvec3(0, 0, 0); // we stop after recursion depth 4
    }
    Hit hit;
    if (!findNearestHitWithAllObjects(ray, scene, hit, 1.0))
    {
        return glm::dvec3(scene.bg_r, scene.bg_g, scene.bg_b); // return background color if the ray doesn't hit anything
    }

    // get intersection point
    glm::dvec3 point = evalRay(ray, hit.t);

    // shoot shadow rays
    glm::dvec3 c_local = shootShadowRays(hit, point, scene, ray, *hit.sphere);

    glm::dvec4 reflected = glm::dvec4(glm::reflect(glm::dvec3(-ray.c), hit.normal), 0.0);
    Ray newRay = Ray{glm::dvec4(point, 1), reflected};

    glm::dvec3 reflectedColor = raytrace(depth + 1, newRay, scene);

    // Ambient
    glm::dvec3 ambient = glm::dvec3(scene.Ir, scene.Ig, scene.Ib) * hit.sphere->Ka * hit.sphere->color;

    return c_local + ambient + reflectedColor * hit.sphere->Kr;
}

void rayTraceAllPixels(const Scene &scene, unsigned char *pixels)
{
    // Origin of the camera
    glm::dvec4 origin(0.0f, 0.0f, 0.0f, 1.0f);

    // Step sizes for each pixel on the near plane
    double dx = static_cast<double>(scene.r - scene.l) / static_cast<double>(scene.x);
    double dy = static_cast<double>(scene.t - scene.b) / static_cast<double>(scene.y);

    for (int j = 0; j < scene.y; ++j)
    {
        for (int i = 0; i < scene.x; ++i)
        {
            // Compute the pixel position on the near plane
            double px = scene.l + (i + 0.5f) * dx; // Center of the pixel in X
            double py = scene.b + (j + 0.5f) * dy; // Center of the pixel in Y (flip Y-axis)
            double pz = -scene.n;                  // Near plane is at -n in camera space

            // Define the ray
            glm::dvec4 pixelPos(px, py, pz, 1.0f);
            glm::dvec4 direction = pixelPos - origin; // Normalize not necessary. This also allows us to exclude intersections that
                                                      // are in front of the nearplane (t < 1)
            Ray ray = Ray{origin, glm::dvec4(direction.x, direction.y, direction.z, 0.0f)};

            // Determine pixel color
            glm::dvec3 color = raytrace(1, ray, scene);

            int pixOffset = 3 * (i + j * scene.x);

            pixels[pixOffset] = color.x * 255.0f;
            pixels[pixOffset + 1] = color.y * 255.0f;
            pixels[pixOffset + 2] = color.z * 255.0f;
        }
    }
}