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

    //assuming only air-medium interfaces
    if(inRay.n_i == 1) {
        outRay.n_i = n_i;
    }
    else {
        outRay.n_i = 1;
    }
    float sin_theta2_squared = inRay.n_i * inRay.n_i/(outRay.n_i * outRay.n_i) * (1 - (dot) * (dot));
    float cos_theta2_squared = 1 - sin_theta2_squared;

    if(sin_theta2_squared > 1) {
        outRay.origin += normal * EPSILON;
        outRay.direction = inRay.direction - normal * 2 * inRay.direction.dot(normal);
    }
    else {
        float cos_theta = sqrt(cos_theta2_squared);
        float r0 =((inRay.n_i-outRay.n_i)*(inRay.n_i-outRay.n_i)+k*k)/((inRay.n_i+outRay.n_i)*(inRay.n_i+outRay.n_i)+k*k);
        
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
    }

    
    return outRay;
}