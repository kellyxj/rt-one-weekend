#ifndef __GROUND_GRID_H__
#define __GROUND_GRID_H__

#include "material.hpp"

class groundGrid: public Material {
public:
    MaterialType type = MaterialType::groundGrid;

    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
    float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos);

    json serialize();
};

#endif