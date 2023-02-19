#include "glass.hpp"

Color Glass::getColor(vec4 & pos) {
    return this->c;
}

ray Glass::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    vec4 dir = inRay.direction.normalize();
    float dot = dir.dot(normal);
    vec4 parallel = normal * dot;
    vec4 orthog = dir - parallel;

    ray outRay(pos, parallel+orthog);

    //assuming only air-medium interfaces
    if(inRay.n_i == 1) {
        outRay.n_i = n_i;
    }
    else {
        outRay.n_i = 1;
    }
    float sin_theta2_squared = inRay.n_i * inRay.n_i/(outRay.n_i * outRay.n_i) * (1 - (dot) * (dot));
    float cos_theta2_squared = 1 - sin_theta2_squared;

    //total internal reflection case
    if(sin_theta2_squared > 1) {
        outRay.origin += normal * EPSILON;
        outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
    }
    else {
        outRay.origin -= normal * EPSILON;
        outRay.direction = normal * -1 * sqrt(cos_theta2_squared);
        outRay.direction += orthog.normalize() * sqrt(sin_theta2_squared);
    }
    
    return outRay;
}