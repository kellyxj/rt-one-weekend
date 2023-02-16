#ifndef __HIT_H__
#define __HIT_H__

#include "ray.hpp"
#include "materials/material.hpp"

class Hit {
public:
    float t = 1e12;
    //default hit brightness controls amount of ambient occlusion
    float brightness = 2;

    vec4 pos;
    vec4 modelSpacePos;
    vec4 normal;
    ray inRay;

    Material* material;
    Color color;
};

inline std::ostream& operator << (std::ostream &os, const Hit &h) {
    os << "time: " << h.t << "\n";
    os << "world space position " << h.pos << "\n";
    os << "model space position " << h.modelSpacePos << "\n";
    os << "normal vector " << h.normal << "\n";
    os << "incident ray " << h.inRay << "\n";
    os << "color: " << h.color << "\n";

    return os;
}

#endif