#include "baseMaterial.hpp"

Color base::getColor(vec4 & pos) {
    return this->c;
}

ray base::scatter(ray & inRay, vec4 & pos, vec4 & normal) {
    double theta = 2*PI*static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    double phi = 2*PI*static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    vec4 randomDirection = pos;
    randomDirection.x += cos(phi) * sin(theta);
    randomDirection.y += sin(phi) * sin(theta);
    randomDirection.z += cos(theta);
    randomDirection.w = 0;

    if(randomDirection.dot(normal) < 0) {
        randomDirection *= -1;
    }
    ray outRay(pos, randomDirection);
    return outRay;
}