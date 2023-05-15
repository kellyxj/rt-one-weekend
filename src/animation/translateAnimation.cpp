#include "translateAnimation.hpp"

mat4 TranslateAnimation::evaluate(float t) {
    mat4 transform;
    transform.setIdentity();
    vec4 translateVec = direction;
    translateVec.normalize();

    translateVec *= speed * t;
    transform = transform.translate(translateVec);

    return transform;
}