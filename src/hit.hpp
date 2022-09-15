#ifndef __HIT_H__
#define __HIT_H__

#include "ray.hpp"
#include "materials/material.hpp"

class Hit {
public:
    double t = 1e12;
    bool isLight = false;

    vec4 pos;
    vec4 modelSpacePos;
    vec4 normal;
    ray inRay;

    Material* material;
    Color color;
};

inline std::ostream& operator << (std::ostream &os, const Hit &h) {
    os << "time: " << h.t << "\n";
    os << "hit light? " << h.isLight << "\n";
    os << "world space position " << h.pos << "\n";
    os << "model space position " << h.modelSpacePos << "\n";
    os << "normal vector " << h.normal << "\n";
    os << "incident ray " << h.inRay << "\n";
    os << "color: " << h.color << "\n";

    return os;
}

#endif