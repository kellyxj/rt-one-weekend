#ifndef __HIT_H__
#define __HIT_H__

#include "vec4.hpp"

class Hit {
public:
    double t;
    vec4 pos;
    vec4 normal;
    //points to origin of incident ray
    vec4 v;
};

#endif