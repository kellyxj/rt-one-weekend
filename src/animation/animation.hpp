#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "../math/mat4.hpp"

class Animation {
public:
    float speed = 0;

    virtual mat4 evaluate(float t) = 0;
};

#endif