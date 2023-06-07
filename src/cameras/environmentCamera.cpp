#include "environmentCamera.hpp"

void EnvironmentCamera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

void EnvironmentCamera::setLookDirection(float pan, float tilt) {
    panAngle = PI*pan/180.0;
    tiltAngle = PI*tilt/180.0;
}

void EnvironmentCamera::setFlips() {
    flipX = true;
    flipY = false;
}

ray EnvironmentCamera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    // azimuthal angle
    float phi = 2 * PI * posU + panAngle; 

    // inclination wrt +z axis
    float theta = PI * posV + tiltAngle;

    ray eyeRay;
    eyeRay.origin = eyePoint;

    // Since the inclination angle starts along +z, we negate the z component
    eyeRay.direction = vec4(sin(theta) * cos(phi), sin(theta) * sin(phi), -cos(theta));
    
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