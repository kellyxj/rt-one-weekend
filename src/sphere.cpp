#include "sphere.hpp"

Hit Sphere::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    vec4 center(0,0,0,1);
    vec4 r2s = center-origin;

    double L2 = r2s.length();
    double tcaS = dir.dot(r2s);

    Hit hit;
    //rays originating inside sphere
    if(L2 <= 1) {
        double DL2 = dir.length();

        double tca2 = tcaS * tcaS/DL2;
        double LM2 = L2 - tca2;

        double L2hc = 1-LM2;
        double t0 = tcaS/DL2 + sqrt(L2hc/DL2);

        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = origin + (dir * t0);
        hit.normal = this->getNormal(hit.modelSpacePos);
        hit.material = this->material;
        hit.v = (hit.pos-inRay.origin).normalize();
    }
    //if tcaS < 0, sphere is behind the camera
    else if(tcaS >= 0) {
        double DL2 = dir.length();

        double tca2 = tcaS * tcaS/DL2;
        double LM2 = L2 - tca2;

        if(LM2 <= 1) {
            double L2hc = 1-LM2;
            double t0 = tcaS/DL2 - sqrt(L2hc/DL2);
            double t1 = tcaS/DL2 + sqrt(L2hc/DL2);

            hit.t = t0;
            hit.pos = inRay.origin + (inRay.direction * t0);
            hit.modelSpacePos = origin + (dir * t0);
            hit.normal = this->getNormal(hit.modelSpacePos);
            hit.material = this->material;
            hit.v = (hit.pos-inRay.origin).normalize();
        }
    }
    return hit;
}

vec4 Sphere::getNormal(vec4 & pos) {
    vec4 normVec(pos.x, pos.y, pos.z, 0);
    normVec = (this->worldToModel).transform(normVec);
    return normVec;
}

int Sphere::getType() {
    return 2;
}