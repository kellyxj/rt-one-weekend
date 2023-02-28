#include "camera.hpp"

#include "defines.hpp"

void Camera::setEyePosition(vec4 pos) {
    eyePoint = pos;
}

void Camera::setLookDirection(float pan, float tilt) {
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
void Camera::rayFrustum(float _left, float _right, float _top, float _bottom, float _near) {
    left = _left;
    right = _right;
    top = _top;
    bottom = _bottom;
    near = _near;
}

void Camera::rayPerspective(float fovy, float aspect, float _near) {
    float _top = _near * tan(.5 * fovy * PI/180);
    rayFrustum(-_top * aspect, _top * aspect, _top, -_top, _near);
}

//return the ray from the camera's aperture to (xPos, yPos) on the image/near plane
//NB: in the image plane's coordinate system, each pixel is one unit wide by one unit tall
ray Camera::getEyeRay(float xPos, float yPos) {
    float posU = left + xPos * pixelWidth;
    float posV = bottom + yPos * pixelHeight;

    ray eyeRay;
    eyeRay.origin = eyePoint;
    eyeRay.direction = (uAxis * posU) + (vAxis * posV) - (nAxis * near);
    eyeRay.direction.normalize();
    return eyeRay;
}

void Camera::setUVN() {
    nAxis = (eyePoint - aimPoint).normalize();
    uAxis = (up.cross(nAxis)).normalize();
    vAxis = nAxis.cross(uAxis);
}

//Camera::lookAt(vec4 aim)

json Camera::serialize() {
    json json_ = {
        {"width", width},
        {"height", height},
        {"eyePoint", eyePoint.serialize()},
        {"aimPoint", aimPoint.serialize()},
        {"up", up.serialize()}
    };

    return json_;
}

Camera Camera::deserialize(json json_) {
    Camera cam;
    
    vec4 eyePoint;
    auto eyePoint_ = json_["eyePoint"];
    eyePoint = eyePoint.deserialize(eyePoint_);
    cam.eyePoint = eyePoint;

    vec4 aimPoint;
    auto aimPoint_ = json_["aimPoint"];
    aimPoint = aimPoint.deserialize(aimPoint_);
    cam.aimPoint = aimPoint;

    vec4 up;
    auto up_ = json_["up"];
    up = up.deserialize(up_);
    cam.up = up;

    cam.setUVN();

    return cam;
}