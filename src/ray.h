#ifndef __RAY_H__
#define __RAY_H__
#include "vec4.h"

class ray {
public:
    vec4 origin;
    vec4 direction;
    int bouncesLeft;

    ray(): origin(vec4()), direction(vec4()), bouncesLeft(-1) {

    }
    ray(vec4 o, vec4 dir, int bounces): origin(o), bouncesLeft(bounces){
        direction = dir.normalize();
    }
};

#endif