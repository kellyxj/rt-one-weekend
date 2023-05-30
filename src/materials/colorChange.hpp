#ifndef __COLORCHANGE_H__
#define __COLORCHANGE_H__

#include "material.hpp"

class ColorChange: public Material {
public:
    MaterialType type = MaterialType::diffuse;

    Color c;
    Color getColor(vec4 & pos, float t);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
    float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos);

    json serialize();
};

#endif