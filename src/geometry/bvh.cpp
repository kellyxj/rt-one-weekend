#include "bvh.hpp"
BVH::BVH() {
    min = vec4();
    max = vec4();
}

BVH::BVH(vec4 & min_, vec4 & max_) {
    min = vec4(min_.x, min_.y, min_.z, min_.w);
    max = vec4(max_.x, max_.y, max_.z, max_.w);
}

void BVH::setMaterial(Material & m) {
    for(Geometry* child : children) {
        child->setMaterial(m);
    }
}

//this should never be called
vec4 BVH::getNormal(vec4 & pos, ray & inRay) {
    vec4 normal;
    return normal;
}

//perform desired transformations on a mesh object, then call mesh.constructBVH
Hit BVH::trace(ray & inRay) {
    Hit closest;
    vec4 inv_dir(1.0/inRay.direction.x, 1.0/inRay.direction.y, 1.0/inRay.direction.z);

    float tx_min = (min.x - inRay.origin.x)*inv_dir.x;
    float ty_min = (min.y - inRay.origin.y)*inv_dir.y;
    float tz_min = (min.z - inRay.origin.z)*inv_dir.z;

    float tx_max = (max.x - inRay.origin.x)*inv_dir.x;
    float ty_max = (max.y - inRay.origin.y)*inv_dir.y;
    float tz_max = (max.z - inRay.origin.z)*inv_dir.z;

    float tx_enter = std::min(tx_min, tx_max);
    float tx_exit = std::max(tx_min, tx_max);

    float ty_enter = std::min(ty_min, ty_max);
    float ty_exit = std::max(ty_min, ty_max);

    float tz_enter = std::min(tz_min ,tz_max);
    float tz_exit = std::max(tz_min ,tz_max);

    float t_enter = std::max(std::max(tx_enter, ty_enter), tz_enter);
    float t_exit = std::min(std::min(tx_exit, ty_exit), tz_exit);

    if(t_enter < t_exit && t_exit > 0) {
        for(Geometry* child : children) {
            Hit hit = child->trace(inRay);
            if(hit.t < closest.t) {
                closest = hit;
            }
        }
    }
    return closest;
}