#include "thinLensCamera.hpp"
#include <iostream>

void ThinLensCamera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

void ThinLensCamera::setLookDirection(float pan, float tilt) {
    tiltAngle = tilt;
    panAngle = pan;
    aimPoint = vec4(eyePoint.x+cos(PI*panAngle/180)*cos(PI*tiltAngle/180),
                    eyePoint.y+sin(-PI*panAngle/180)*cos(PI*tiltAngle/180),
                    eyePoint.z+sin(PI*tiltAngle/180),
                    1);
    up = vec4(cos(PI*panAngle/180)*cos(PI*(tiltAngle+90)/180),
                    sin(-PI*panAngle/180)*cos(PI*(tiltAngle+90)/180),
                    sin(PI*(tiltAngle+90)/180),
                    0);

    setUVN();
}

//define the absolute size of the image plane and its distance from the aperture
void ThinLensCamera::rayFrustum(float _left, float _right, float _top, float _bottom, float _near) {
    left = _left;
    right = _right;
    top = _top;
    bottom = _bottom;
    near = _near;
}

void ThinLensCamera::rayPerspective(float fovy, float aspect, float _near) {
    float _top = _near * tan(.5 * fovy * PI/180);
    rayFrustum(-_top * aspect, _top * aspect, _top, -_top, _near);
}

//return the ray from the camera's aperture to (xPos, yPos) on the image/near plane
//NB: in the image plane's coordinate system, each pixel is one unit wide by one unit tall
ray ThinLensCamera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    ray eyeRay;
    eyeRay.origin = eyePoint + (uAxis * posU) + (vAxis * posV) + (nAxis * near);
    eyeRay.direction = eyePoint - eyeRay.origin;
    eyeRay.direction.w = 0;

    if (apertureRadius > 0) {
        // Determine the point where the ray hit the lens
        Posn2D circleSample = uniformDiskSample();
        float circleX = apertureRadius * circleSample.x;
        float circleY = apertureRadius * circleSample.y;

        // Compute where that point lies on the plane of focus
        float ft = focalDistance / sqrt((eyePoint - aimPoint).length_squared());
        vec4 pFocus = eyeRay.origin + (eyeRay.direction * ft);

        // Update the ray according to computed thin lens effect
        eyeRay.origin = eyePoint + (uAxis * circleX) + (vAxis * circleY);
        eyeRay.direction = pFocus - eyeRay.origin;
    }

    eyeRay.direction = eyeRay.direction.normalize();
    return eyeRay;
}

void ThinLensCamera::setUVN() {
    nAxis = (eyePoint - aimPoint).normalize();
    uAxis = (up.cross(nAxis)).normalize();
    vAxis = nAxis.cross(uAxis);
}

//Camera::lookAt(vec4 aim)

json ThinLensCamera::serialize() {
    json json_ = {
        {"width", width},
        {"height", height},
        {"eyePoint", eyePoint.serialize()},
        {"aimPoint", aimPoint.serialize()},
        {"up", up.serialize()},
        {"left", left},
        {"right", right}, 
        {"top", top},
        {"bottom", bottom},
        {"near", near},
        {"exposure", exposure},
        {"gamma", gamma}
    };

    return json_;
}

// Naive disk sample keeps generating random positions in unit square until 
// it generates a point that lies in the unit circle
Posn2D naiveDiskSample() {
    while (true) { // This could be dangerous...
        float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if (randX*randX + randY*randY <= 1) {
            return Posn2D(randX, randY);
        }
    }
}

// Generates a uniform sampled position on a unit disk by assuming uniform area concentric rings
// The side effect of this is that area is not mapped uniformally. (i.e. outer rings are thinner)
Posn2D uniformDiskSample() {
    float r = sqrt(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    float theta = 2 * PI * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return Posn2D(r * cos(theta), r * sin(theta));
}