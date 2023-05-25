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

// Generate a ray that passes through a simulated thin lens at pixel xPos, yPos
ray ThinLensCamera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    ray eyeRay;
    // We are assuming the film plane lies in front of the camera origin
    eyeRay.origin = eyePoint;
    eyeRay.direction = (uAxis * posU) + (vAxis * posV) - (nAxis * near);
    eyeRay.direction.w = 0;

    if (apertureRadius > 0) {
        // Generate the point where the ray hit the lens
        Point2f circleSample = uniformDiskSample();
        float circleX = apertureRadius * circleSample.x;
        float circleY = apertureRadius * circleSample.y;

        // Compute where the ray from that point lies on the plane of focus
        float ft = focalDistance / sqrt((eyePoint - aimPoint).length_squared());
        vec4 pFocus = eyeRay.origin + (eyeRay.direction * ft);

        // * Update the ray according to computed thin lens effect
        // The ray now originates at the point where it hit the lens
        eyeRay.origin = eyePoint + (uAxis * circleX) + (vAxis * circleY);
        // And moves towards the point on the plane of focus
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