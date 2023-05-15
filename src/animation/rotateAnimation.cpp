#include "rotateAnimation.hpp"

mat4 RotateAnimation::evaluate(float t) {
    mat4 transform;
    transform.setIdentity();

    // for now, rotation speed is 90 deg per t
    float angle = speed * 90 * t;
    
    transform = transform.rotate(angle, axis);

    return transform;
}