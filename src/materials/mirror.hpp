#ifndef __MIRROR_H__
#define __MIRROR_H__

#include "material.hpp"

class Mirror: public Material {
public:
    float r0; //reflectance at normal incidence
    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
};

#endif