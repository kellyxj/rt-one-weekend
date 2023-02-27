#ifndef __DIFFUSE_H__
#define __DIFFUSE_H__

#include "material.hpp"

class base: public Material {
public:
    MaterialType type = MaterialType::diffuse;

    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
    float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos);

    json serialize();
    Material * deserialize(json json_);
};

#endif