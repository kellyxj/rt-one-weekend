#ifndef __MIRROR_H__
#define __MIRROR_H__

#include "material.hpp"

class Mirror: public Material {
public:
    MaterialType type = MaterialType::conductor;

    float r0; //reflectance at normal incidence

    float a = 1.136; //regression coefficients
    float alpha = 6;

    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);

    json serialize();
    Material * deserialize(json json_);
};

#endif