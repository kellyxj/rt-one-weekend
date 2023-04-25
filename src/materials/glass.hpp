#ifndef __GLASS_H__
#define __GLASS_H__

#include "material.hpp"

class Glass: public Material {
public:
    MaterialType type = MaterialType::dielectric;
    bool isDelta =true;

    float n_i;
    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
    float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos);

    json serialize();
};

#endif