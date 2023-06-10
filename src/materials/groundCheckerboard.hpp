#ifndef __GROUND_CHECKERBOARD_H__
#define __GROUND_CHECKERBOARD_H__

#include "material.hpp"

class groundCheckerboard: public Material {
public:
    MaterialType type = MaterialType::groundCheckerboard;

    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
    float sampleBrdf(ray &inRay, ray& outRay, vec4 &pos);

    json serialize();
};

#endif