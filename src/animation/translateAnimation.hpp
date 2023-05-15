#include "animation.hpp"
#include <iostream>


class TranslateAnimation : public Animation {
public:

    vec4 direction;

    TranslateAnimation(vec4 direction_, float speed_) {
        direction = direction_;
        speed = speed_;
    }

    mat4 evaluate(float t);
};