#ifndef __BVH_H__

#include "geometry.hpp"
#include <vector>
#include <algorithm>

enum class SplitAxis {x, y, z};

//axis-aligned bounding box
//since box is axis-aligned, we only need to specify the two opposite corners (min and max)
class BVH : public Geometry {
public:
    GeometryType type = GeometryType::bvh;

    vec4 min;
    vec4 max;
    vec4 centroid;
    std::vector<Geometry*> children;

    BVH();
    BVH(vec4 & min_, vec4 & max_);

    vec4 computeCentroid();
    void setMaterial(Material & m);
    vec4 getNormal(vec4 & pos, ray & inRay);
    Hit trace(ray & inRay);

    //expand the bounding box to include point
    BVH include(vec4 & point);

    //combine this bounding box with a different one
    BVH coalesce(BVH & other);

    BVH transform(mat4 & m);

    BVH build(int maxDepth);

    BVH buildRecursive(int currentDepth, int maxDepth);

    json serialize();
};

#endif