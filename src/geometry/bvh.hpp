#ifndef __BVH_H__

#include "geometry.hpp"
#include <vector>

//axis-aligned bounding box
//since box is axis-aligned, we only need to specify the two opposite corners (min and max)
class BVH : public Geometry {
public:
    vec4 min;
    vec4 max;
    std::vector<Geometry*> children;

    BVH();
    BVH(vec4 & min_, vec4 & max_);

    void setMaterial(Material & m);
    vec4 getNormal(vec4 & pos, ray & inRay);
    Hit trace(ray & inRay);
};

#endif