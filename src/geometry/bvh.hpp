#ifndef __BVH_H__

#include "geometry.hpp"

//axis-aligned bounding box
class BVH : public Geometry {
public:
    vec4 smallCorner;
    vec4 bigCorner;
    BVH();
    BVH(vec4 & min, vec4 & max);
};

#endif