#ifndef __HIT_H__
#define __HIT_H__

#include "ray.hpp"
#include "material.hpp"

class Hit {
public:
    double t = 1e12;
    vec4 pos;
    vec4 modelSpacePos;
    vec4 normal;
    //points to origin of incident ray
    vec4 v;

    Material* material;
};

#endif