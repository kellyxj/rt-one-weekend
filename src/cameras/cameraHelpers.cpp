#include "cameraHelpers.hpp"
#include <iostream>

Point2f uniformDiskSample() {
    float r = sqrt(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    float theta = 2 * PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return Point2f(r * cos(theta), r * sin(theta));
}

bool quadratic(float a, float b, float c, float *t0, float *t1) {
    // use doubles to minimize floating point error, especially for sqrt
    double discriminant = (double)b*(double)b - 4*(double)a*(double)c;
    if (discriminant < 0) return false;
    double rootDiscriminant = sqrt(discriminant); // must be positive

    double q;
    if (b < 0) q = -0.5 * (b - rootDiscriminant); // q must be positive
    else q = -0.5 * (b + rootDiscriminant); // q must be negative

    *t0 = q/a;
    *t1 = c/q;

    // make sure t0 < t1
    if (*t0 > *t1) std::swap(*t0, *t1);
    return true;
}

bool refract(const vec4 &wi, const vec4 &n, float eta, vec4 *wt) {
    vec4 n_ = n;
    vec4 wi_ = wi;
    float cosThetaI = n_.dot(wi_);

    float sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
    float sin2ThetaT = eta*eta*sin2ThetaI;

    // ignore total internal reflection case
    if (sin2ThetaT >= 1) return false;

    float cosThetaT = sqrt(1 - sin2ThetaT);

    *wt = (wi_*-1)*eta + n_*(eta*cosThetaI - cosThetaT);

    return true; 
}

float lerp(float t, float v1, float v2) {
    return (1-t)*v1 + t*v2;
}