#include "translateAnimation.hpp"

mat4 TranslateAnimation::evaluate(float t) {
    mat4 transform;
    transform.setIdentity();
    vec4 translateVec(0,0,0);
    switch(direction) {
        case 0:
            translateVec.x += 1;
            break;
        case 1:
            translateVec.y += 1;
            break;
        case 2:
            translateVec.z += 1;
            break;
        default:
            break;
    }

    translateVec *= speed * t;
    transform = transform.translate(translateVec);

    return transform;
}