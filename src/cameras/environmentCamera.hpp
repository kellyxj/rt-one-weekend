#ifndef __ENVIRONMENT_CAMERA_H__
#define __ENVIRONMENT_CAMERA_H__

#include "camera.hpp"

class EnvironmentCamera : public Camera {
public:
    vec4 eyePoint;
    
    // stored internally as radians
    float panAngle;
    // stored internally as radians
    float tiltAngle;

    float left = 0;
    float right = 1;
    float bottom = 0;
    float top = 1;

    EnvironmentCamera() {
        eyePoint = vec4(-10, 0, 1, 1);

        panAngle = 0;
        tiltAngle = 0;

        width = 512;
        height = 512;
        pixelWidth = (float)(right - left)/(float)(width);
        pixelHeight = (float)(top - bottom)/(float)(height);

        setFlips();
    }

    // Input pan and tilt as degrees!
    // Keep the tilt at 0 or things go wrong...
    EnvironmentCamera(vec4 eye, int w, int h, float pan, float tilt): eyePoint(eye) {
        panAngle = PI*pan/180.0;
        tiltAngle = PI*tilt/180.0;

        width = w;
        height = h;
        pixelWidth = (float)(right - left)/(float)(width);
        pixelHeight = (float)(top - bottom)/(float)(height);

        setFlips();
    }

    void setEyePosition(vec4 pos);
    
    // Input pan and tilt as degrees!
    void setLookDirection(float pan, float tilt);

    void setFlips();

    ray getEyeRay(float xPos, float yPos);

    json serialize();

    Camera* deserialize(json json_);
};

#endif