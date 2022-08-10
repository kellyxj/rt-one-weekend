#pragma once
#include "vec4.h"

class ray {
public:
    vec4 origin;
    vec4 direction;
    int bouncesLeft;

    Ray: origin(Vector()), direction(Vector()), bouncesLeft(-1) {

    }
    Ray(Vector o, Vector dir, int bounces): origin(o), bouncesLeft(bounces){
        direction = dir.normalize();
    }
};