#include "mirror.hpp"

Color Mirror::getColor(vec4 & pos) {
    return this->c;
}

ray Mirror::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    ray outRay;
    outRay.origin = pos;

    vec4 dir = inRay.direction.normalize();
    float dot = dir.dot(normal);
    vec4 parallel = normal * dot;
    vec4 orthog = dir - parallel;

    float cos_theta = -dot;
    
    float a = 3;
    float alpha = 6;
    float reflectedPercent = r0 + (1 - r0)*pow((1 - cos_theta), 5)-a *cos_theta*pow((1 - cos_theta),alpha);

    double intPart;
    float randomVariable = ((float)rand()/RAND_MAX);

    if(randomVariable > reflectedPercent) {
        outRay.origin -= normal * EPSILON;
        outRay.direction = vec4();
    }
    else {
        outRay.origin += normal * EPSILON;
        outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
    }

    
    return outRay;
}