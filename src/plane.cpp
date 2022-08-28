#include "plane.hpp"

Hit Plane::trace(ray & inRay) {
    double t0 = -inRay.origin.z/inRay.direction.z;
    Hit hit;
    if(t0 >= 0) {
        hit.t = t0;
        hit.pos = inRay.origin;
        hit.pos += inRay.direction * t0;
        hit.normal = this->getNormal(hit.pos);
    }
    return hit;
}
vec4 Plane::getNormal(vec4 & pos) {
    return vec4(0,0,1,0);
}