#include "ray_utils.h"

#include <cmath>  // For std::sqrt

bool solveQuadratic(double a, double b, double c, double &x1, double &x2) {
    if (a == 0) {  // Degenerate case: Not a quadratic equation
        if (b == 0) {
            return false;  // No solution if a and b are 0
        }
        x1 = x2 = -c / b;  // Linear solution
        return true;
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;  // No real solutions
    } else if (discriminant == 0) {
        x1 = x2 = -b / (2 * a);  // One real solution
    } else {
        double sqrtDiscriminant = std::sqrt(discriminant);
        x1 = (-b + sqrtDiscriminant) / (2 * a);
        x2 = (-b - sqrtDiscriminant) / (2 * a);
    }

    return true;
}