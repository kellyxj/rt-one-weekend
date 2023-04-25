#include "animation.hpp"
#include <iostream>

class TranslateAnimation : public Animation {
public:
    TranslateAnimation(int direction_, float speed_) {
        direction = direction_;  //0 means +x, 1 means +y, 2 means +z
        speed = speed_;
    }

    mat4 evaluate(float t);
};