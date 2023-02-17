#include "square.hpp"

Hit Square::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    float t0 = -origin.z/dir.z;
    vec4 modelSpacePos = origin + (dir * t0);

    Hit hit;
    if(t0 >= 0 && (modelSpacePos.x > -1 && modelSpacePos.x < 1 && modelSpacePos.y > -1 && modelSpacePos.y < 1)) {
        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = modelSpacePos;
        hit.normal = this->getNormal(hit.pos, ray);
        hit.material = this->material;
        hit.brightness = hit.material->brightness;
        hit.inRay = inRay;
    }
    return hit;
}
vec4 Square::getNormal(vec4 & pos, ray & inRay) {
    vec4 v(0,0,1,0);
    if(v.dot(inRay.direction) > 0) {
        v.z = -1;
    }
    return (this->normalToWorld).transform(v);
}