#ifndef __CAMERAHELPERS_H__
#define __CAMERAHELPERS_H__

#include <cmath>
#include "../defines.hpp"
#include "../math/vec4.hpp"

struct Point2f {
    float x;
    float y;
    Point2f(float x, float y): x(x), y(y) {};

    // Default
    Point2f() {
        x = .0;
        y = .0;
    };
};

struct Bounds2f {
    Point2f x;
    Point2f y;
    Bounds2f(Point2f x, Point2f y): x(x), y(y) {};
    
    // Default
    Bounds2f() {
        x = Point2f(.0,.0); 
        y = Point2f(.0,.0);
    };
};

// Generate a unifromatlly sampled point on a 2D disk.
// This method assumes rings of equal area have equal probability.
Point2f uniformDiskSample();

// Solves quadratic equations of the form ax^2 + bx + c = 0 using 
// a "stable" version of the quadratic formula.
// Stores the result(s) in t0, t1. 
// Returns true if discriminant is positive, else false.
bool quadratic(float a, float b, float c, float *t0, float *t1);

// Computes new direction for incoming vector wi which is stored in wt
// Note: eta represents the index of refraction
// Note: returns false if the ray is totally internally reflects, else true
bool refract (const vec4 &wi, const vec4 &n, float eta, vec4 *wt);

// Linearly interpolate between two provided values
float lerp(float t, float v1, float v2);

// ParallelFor loop implementation closely following pbrt's 
// implementation (like much of the rest of this)
// For more info see pbrt's section A.6.4
// TODO

#endif