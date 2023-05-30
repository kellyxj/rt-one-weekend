#include "pinholeCamera.hpp"

void PinholeCamera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

void PinholeCamera::setLookDirection(float pan, float tilt) {
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
void PinholeCamera::rayFrustum(float _left, float _right, float _top, float _bottom, float _near) {
    left = _left;
    right = _right;
    top = _top;
    bottom = _bottom;
    near = _near;
}

void PinholeCamera::rayPerspective(float fovy, float aspect, float _near) {
    float _top = _near * tan(.5 * fovy * PI/180);
    rayFrustum(-_top * aspect, _top * aspect, _top, -_top, _near);
}

//return the ray from the camera's aperture to (xPos, yPos) on the image/near plane
//NB: in the image plane's coordinate system, each pixel is one unit wide by one unit tall
ray PinholeCamera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    ray eyeRay;
    eyeRay.origin = eyePoint;
    eyeRay.direction = (uAxis * posU) + (vAxis * posV) - (nAxis * near);
    eyeRay.direction.normalize();
    return eyeRay;
}

void PinholeCamera::setUVN() {
    nAxis = (eyePoint - aimPoint).normalize();
    uAxis = (up.cross(nAxis)).normalize();
    vAxis = nAxis.cross(uAxis);
}

void PinholeCamera::matSetEyePosition(mat4 transform) {

    mat4 eyePtMat = mat4();
    eyePtMat.entries[0] = eyePoint.x; eyePtMat.entries[1] = eyePoint.y; eyePtMat.entries[2] = eyePoint.z; eyePtMat.entries[3] = 1;
    eyePtMat = transform.multiply(eyePtMat);
    eyePoint.x = eyePtMat.entries[0]; eyePoint.y = eyePtMat.entries[1]; eyePoint.z = eyePtMat.entries[2];

    if (!holdAimPt) {
        mat4 aimPtMat = mat4();
        aimPtMat.entries[0] = aimPoint.x; aimPtMat.entries[1] = aimPoint.y; aimPtMat.entries[2] = aimPoint.z; aimPtMat.entries[3] = 1;
        aimPtMat = transform.multiply(aimPtMat);
        aimPoint.x = aimPtMat.entries[0]; aimPoint.y = aimPtMat.entries[1]; aimPoint.z = aimPtMat.entries[2];
    }

}

vec4 PinholeCamera::getEyePoint() {
    return eyePoint;
}

//Camera::lookAt(vec4 aim)

json PinholeCamera::serialize() {
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
