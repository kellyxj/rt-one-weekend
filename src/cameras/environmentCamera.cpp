#include "environmentCamera.hpp"

void EnvironmentCamera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

ray EnvironmentCamera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    float theta = PI * posV;
    float phi = 2 * PI * posU;

    ray eyeRay;
    eyeRay.origin = eyePoint;
    eyeRay.direction = vec4(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    return eyeRay;
}

json EnvironmentCamera::serialize() {
    json json_ = {
        {"width", width},
        {"height", height},
        {"eyePoint", eyePoint.serialize()},
        {"left", left},
        {"right", right}, 
        {"top", top},
        {"bottom", bottom},
        {"exposure", exposure},
        {"gamma", gamma}
    };

    return json_;
}