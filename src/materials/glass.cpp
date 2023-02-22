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
        //Schlick approximation https://en.wikipedia.org/wiki/Schlick%27s_approximation
        float cos_theta = sqrt(cos_theta2_squared);
        float r0 = (inRay.n_i-outRay.n_i)*(inRay.n_i-outRay.n_i)/((inRay.n_i+outRay.n_i)*(inRay.n_i+outRay.n_i));
        float reflectedPercent = r0+(1-r0)*(1-cos_theta);

        float randomVariable = rand()%1;
        if(randomVariable > reflectedPercent) {
            outRay.origin -= normal * EPSILON;
            outRay.direction = normal * -1 * cos_theta;
            outRay.direction += orthog.normalize() * sqrt(sin_theta2_squared);
        }
        else {
            outRay.origin += normal * EPSILON;
            outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
        }
    }
    
    return outRay;
}