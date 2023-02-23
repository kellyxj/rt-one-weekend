#ifndef __MIRROR_H__
#define __MIRROR_H__

#include "material.hpp"

class Mirror: public Material {
public:
    float n_i;
    Color c;
    Color getColor(vec4 & pos);
    ray scatter(ray & inRay, vec4 & pos, vec4 & normal);
};

#endif