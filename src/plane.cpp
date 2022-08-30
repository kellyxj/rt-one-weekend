#include "plane.hpp"
#include <iostream>

Hit Plane::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);
    dir = dir.normalize();
    ray ray(origin, dir, inRay.bouncesLeft);

    double t0 = -ray.origin.z/ray.direction.z;
    Hit hit;
    if(t0 >= 0) {
        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction.normalize() * t0);
        hit.modelSpacePos = ray.origin + (ray.direction * t0);
        hit.normal = this->getNormal(hit.pos);
    }
    return hit;
}
vec4 Plane::getNormal(vec4 & pos) {
    vec4 v(0,0,1,0);
    return (this->normalToWorld).transform(v);
}