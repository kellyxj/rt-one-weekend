#include "animation.hpp"
#include <iostream>


class RotateAnimation : public Animation {
public:

    vec4 axis;

    RotateAnimation(vec4 axis_, float speed_) {
        axis = axis_; 
        speed = speed_;
    }

    mat4 evaluate(float t);
};