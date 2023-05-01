#ifndef __ENVIRONMENT_CAMERA_H__
#define __ENVIRONMENT_CAMERA_H__

#include "camera.hpp"

class EnvironmentCamera : public Camera {
public:
    vec4 eyePoint;
    
    float left;
    float right;
    float top;
    float bottom;

    EnvironmentCamera() {
        eyePoint = vec4(-10, 0, 1, 1);

        left = 0;
        right = 1;
        top = 1;
        bottom = 0;

        width = 512;
        height = 512;
        pixelWidth = (float)(right - left)/(float)(width);
        pixelHeight = (float)(top - bottom)/(float)(height);
    }

    EnvironmentCamera(vec4 eye, int w, int h): eyePoint(eye) {

        eyePoint = vec4(-10, 0, 1, 1);

        left = 0;
        right = 1;
        top = 1;
        bottom = 0;

        width = w;
        height = h;

        pixelWidth = (float)(right - left)/(float)(width);
        pixelHeight = (float)(top - bottom)/(float)(height);
    }

    void setEyePosition(vec4 pos);

    ray getEyeRay(float xPos, float yPos);

    json serialize();

    Camera* deserialize(json json_);
};

#endif